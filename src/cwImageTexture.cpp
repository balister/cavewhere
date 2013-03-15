//Our includes
#include "cwImageTexture.h"
#include "cwImageProvider.h"
#include "cwTextureUploadTask.h"
#include "cwDebug.h"

//QT includes
#include <QtConcurrentRun>
#include <QtConcurrentMap>
#include <QVector2D>
#include <QWindow>

QThread* cwImageTexture::TextureLoadingThread = NULL;

/**

  */
cwImageTexture::cwImageTexture(QObject *parent) :
    QObject(parent),
    TextureDirty(false),
    TextureId(0),
    LoadNoteWatcher(NULL),
    TextureUploadTask(NULL)
{
    if(TextureLoadingThread == NULL) {
        TextureLoadingThread = new QThread();
        TextureLoadingThread->start(QThread::LowPriority);
    }
}

cwImageTexture::~cwImageTexture()
{
    if(LoadNoteWatcher != NULL) {
        LoadNoteWatcher->deleteLater();
    }

    if(TextureId != 0) {
        glDeleteTextures(1, &TextureId);
    }
}

/**
  This initilizes the texture map in opengl
  */
void cwImageTexture::initialize()
{
    glGenTextures(1, &TextureId);
    glBindTexture(GL_TEXTURE_2D, TextureId);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //LINEAR_MIPMAP_LINEAR);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //GL_NEAREST); //LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    glBindTexture(GL_TEXTURE_2D, 0);
}

/**
Sets project
*/
void cwImageTexture::setProject(QString project) {
    if(ProjectFilename != project) {
        ProjectFilename = project;
        startLoadingImage();
        emit projectChanged();
    }
}

/**
 * @brief cwImageTexture::scaleTexCoords
 * @return  How the text are should be scaled
 */
QVector2D cwImageTexture::scaleTexCoords() const
{
    return ScaleTexCoords;
}

/**
Sets image
*/
void cwImageTexture::setImage(cwImage image) {
    if(Image != image) {
        Image = image;
        startLoadingImage();
        emit imageChanged();
    }
}

/**
  This upload the results from texture image to the graphics card
  */
void cwImageTexture::updateData() {
    if(!isDirty()) { return; }

    if(TextureUploadTask == NULL) {
        TextureDirty = false;
        return;
    }

    QList<QPair<QByteArray, QSize> > mipmaps = TextureUploadTask->mipmaps();
    ScaleTexCoords = TextureUploadTask->scaleTexCoords();

    if(mipmaps.empty()) { return; }

    QSize firstLevel = mipmaps.first().second;
    if(!cwTextureUploadTask::isDivisibleBy4(firstLevel)) {
        qDebug() << "Trying to upload an image that isn't divisible by 4. This will crash ANGLE on windows." << LOCATION;
        TextureDirty = false;
        return;
    }

    //Load the data into opengl
    bind();

    //Get the max texture size
    GLint maxTextureSize;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);

    int trueMipmapLevel = 0;
    for(int mipmapLevel = 0; mipmapLevel < mipmaps.size(); mipmapLevel++) {

        //Get the mipmap data
        QPair<QByteArray, QSize> image = mipmaps.at(mipmapLevel);
        QByteArray imageData = image.first;
        QSize size = image.second;

        if(size.width() < maxTextureSize && size.height() < maxTextureSize) { // &&
                //size.width() >= 4 && size.height() >= 4) {
            glCompressedTexImage2D(GL_TEXTURE_2D, trueMipmapLevel, GL_COMPRESSED_RGB_S3TC_DXT1_EXT,
                                   size.width(), size.height(), 0,
                                   imageData.size(), imageData.data());

            trueMipmapLevel++;
        }
    }

    release();

    reinitilizeLoadNoteWatcher();

    TextureDirty = false;
}

/**
 * @brief cwImageTexture::startLoadingImage
 *
 * Loads the image into the graphics card
 */
void cwImageTexture::startLoadingImage()
{
    if(Image.isValid() && !project().isEmpty()) {

        if(TextureUploadTask == NULL) {
            TextureUploadTask = new cwTextureUploadTask();
            TextureUploadTask->setThread(TextureLoadingThread);

            connect(TextureUploadTask, &cwTextureUploadTask::finished, this, &cwImageTexture::markAsDirty);
            connect(TextureUploadTask, &cwTextureUploadTask::finished, this, &cwImageTexture::textureUploaded);
        }

        if(TextureUploadTask->isRunning()) { return; }

        TextureUploadTask->setImage(image());
        TextureUploadTask->setProjectFilename(ProjectFilename);
        TextureUploadTask->start();
    }
}

/**
 * @brief cwImageTexture::reinitilizeLoadNoteWatcher
 */
void cwImageTexture::reinitilizeLoadNoteWatcher()
{
    if(TextureUploadTask == NULL) {
        TextureUploadTask->deleteLater();
        TextureUploadTask = NULL;
    }
}

void cwImageTexture::markAsDirty()
{
    TextureDirty = true;
}

/**
  \brief Allow QtConturrent to create a QImage

  This returns a opengl formatted image
  */
cwImageTexture::LoadImageData cwImageTexture::LoadImageKernal::operator ()(int imageId) {
    //Extract the image data from the imageProvider
    cwImageProvider imageProvidor;
    imageProvidor.setProjectPath(Filename);

    LoadImageData data;

    QByteArray imageData = imageProvidor.requestImageData(imageId, &data.ImageDataSize);

    data.ImageData = imageData;

    return data;
}



