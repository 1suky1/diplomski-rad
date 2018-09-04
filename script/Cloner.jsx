function Cloner(thisObj)
{
    function buildUI(thisObj)
    {
        var name = "Cloner";
        var version = "v1.0";
        var myPanel = (thisObj instanceof Panel) ? thisObj : new Window("palette", name + " " + version, undefined, {resizeable:true});

        res = "group\
        {\
            orientation:'column',  alignment:['fill','center'], alignChildren:['fill','fill'],\
            groupOffset: Group\
            {\
                orientation:'row', alignChildren:['fill','center'],\
                staticOff: StaticText{text: 'Offset', alignment: 'left'},\
                inOffset: EditText{text:'100', characters:'3'}\
            }\
            groupLabels: Group\
            {\
                orientation:'row', alignChildren:['fill','center'],\
                staticX: StaticText{text: 'X', justify:'center'},\
                staticY: StaticText{text: 'Y', justify:'center'},\
                staticZ: StaticText{text: 'Z', justify:'center'},\
            }\
            groupInputs: Group\
            {\
                orientation:'row', alignChildren:['fill','center'],\
                inX: EditText{text: '1', characters: '2'}\
                inY: EditText{text: '1', characters: '2'}\
                inZ: EditText{text: '1', characters: '2'}\
            }\
            groupClone: Group\
            {\
                orientation:'column', alignChildren:['fill','center'],\
                btnClone: Button{text:'Clone'}\
            }\
        }";

        myPanel.grp = myPanel.add(res);
        
        myPanel.layout.layout(true);
        myPanel.grp.minimumSize = myPanel.grp.size;

        myPanel.layout.resize();
        myPanel.onResizing = myPanel.onResize = function()
        {
            this.layout.resize();
        }
        
        myPanel.grp.groupClone.btnClone.onClick = function()
        {
            var offset = parseInt(myPanel.grp.groupOffset.inOffset.text);
            var inX = parseInt(myPanel.grp.groupInputs.inX.text);
            var inY = parseInt(myPanel.grp.groupInputs.inY.text);
            var inZ = parseInt(myPanel.grp.groupInputs.inZ.text);

            //Check if inputs are correct, if not then alert the user
            if(isNaN(inX) || isNaN(inY) || isNaN(inZ) || isNaN(offset))       
                alert("Input must be a whole number.");
            else
            {   
                StartCloning(inX, inY, inZ, offset);
            }        
        }
        //myPanel.layout.layout(true);
        return myPanel;  
    }

    var myScriptPal = buildUI(thisObj);
   
    if((myScriptPal != null) && (myScriptPal instanceof Window))
    {
        myScriptPal.center();
        myScriptPal.show();
    }
    
    if(this instanceof Panel)
        myScriptPal.show();

    function StartCloning(inX, inY, inZ, offset)
    {
        var selectedLayer = app.project.activeItem.selectedLayers[0];

        if(selectedLayer == null)
            alert("No layer selected.");
        else
        {
            //Set the start of undo point in case user wants to revert
            app.beginUndoGroup("Clone");

            //get selected layer and get it's position
            var selectedPos = selectedLayer.property("Transform").property("Position").value;

            //Create a 3D null layer to be set as center parent
            var centerPoint = app.project.activeItem.layers.addNull();
            centerPoint.threeDLayer = true;
            //Set center point to be the same position as the selected layer, use that for the center of the clone area
            centerPoint.property("Transform").property("Position").setValue(selectedPos);

            //Calculate total dimensions and starting points for the cloing area
            var totalW = ((inX-1) * offset) + (inX * selectedLayer.width);
            var totalH = ((inY-1) * offset) + (inY * selectedLayer.height);
            var totalD = ((inZ-1) * offset);
            var xStart = selectedPos[0] - (totalW / 2);
            var yStart = selectedPos[1] - (totalH / 2);
            var zStart = selectedPos[2] - (totalD / 2);
            
            //Do the cloning
            CloneLayer(inX, inY, inZ, offset, selectedLayer, totalW, totalH, totalD, centerPoint, xStart, yStart, zStart);

            //End of undo point
            app.endUndoGroup();
        }
       
    }

    function CloneLayer(inX, inY, inZ, offset, selectedLayer, totalW, totalH, totalD, centerPoint, xStart, yStart, zStart)
    {
        //if layer is not 3d ignore z value aka set it to 1
        if(!selectedLayer.threeDLayer)
        {
            inZ = 1;
            alert("Selected layer is not 3D, cloning only in X and Y.");
        }
        //Cloning in x y z
        for(x = 0; x < inX; x++)
        {
            for(y = 0; y < inY; y++)
            {
                for(z = 0; z < inZ; z++)
                {
                    //Duplicate the selected layer and get it's position
                    var duplicated = selectedLayer.duplicate();
                    var pos = duplicated.property("Transform").property("Position").value;

                    //Set the new positions and apply it to the duplicated layer
                    pos[0] = xStart + totalW - (totalW - ((inX-x-1) * duplicated.width) - ((inX-x-1) * offset) - duplicated.width/2);
                    pos[1] = yStart + totalH - (totalH - ((inY-y-1) * duplicated.height) - ((inY-y-1) * offset) - duplicated.height/2);
                    pos[2] = zStart + totalD - (totalD - ((inZ-z-1) * offset));
                    duplicated.property("Transform").property("Position").setValue(pos);

                    //Set the centerPoint to be the parent of the duplicated layer
                    duplicated.parent = centerPoint;
                }
            }
        }
        //hide default layer
        selectedLayer.enabled = false;
    } 
}

Cloner(this);