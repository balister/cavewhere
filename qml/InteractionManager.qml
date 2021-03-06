/**************************************************************************
**
**    Copyright (C) 2013 by Philip Schuchardt
**    www.cavewhere.com
**
**************************************************************************/

import QtQuick 2.0
import Cavewhere 1.0

Item {

    property list<Interaction> interactions
    property Interaction defaultInteraction
    property Interaction activeInteraction: null

    //This function hides all other interaction and shows the active interaction
    function active(interaction) {

        //            Make sure the interaction exists
        for(var i = 0; i < interactions.length; i++) {
            var item = interactions[i];
            //Make all interaction invisible
            item.visible = false;
        }

        interaction.visible = true;
        activeInteraction = interaction
    }

    function activeDefaultInteraction() {
        active(defaultInteraction)
    }

    Component.onCompleted: {
        for(var i = 0; i < interactions.length; i++) {
            var interaction = interactions[i];
            interaction.visible = false
            interaction.activated.connect(active);
            interaction.deactivated.connect(activeDefaultInteraction);
        }

        active(defaultInteraction)
    }

}
