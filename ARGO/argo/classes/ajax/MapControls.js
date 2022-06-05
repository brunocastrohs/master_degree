MapControls = {

handleMeasurements:'',
measureControls:'',
printProvider:'',printButton:'',edgeMerger:'',routMerger:'',
zoomBox:'',zoomToContextExtent:'',overViewMap:'',optionsLine:'',optionsPolygon:'',measureControl:'',surroundControl:'',pathControl:'',
box:'',

initControls:function(){
	
	this.box = new OpenLayers.Control.DrawFeature(MapManager.vector,
			OpenLayers.Handler.RegularPolygon, {
        		displayClass: "olControlDrawGeoCodBox",
				handlerOptions : {
					sides : 4,
					snapAngle : 90,
					irregular : true,
					//persist : true
				}
			});
	
	this.box.handler.callbacks.done = MapAction.drawGeocodBox;

	this.box.handler.callbacks.create = function(){MapManager.vector.removeAllFeatures();};
	
	//SVN - Printcapabilities
	if(printCapabilities)
		this.printProvider = new GeoExt.data.PrintProvider({
        method: "GET", // "POST" recommended for production use
        capabilities: printCapabilities, // provide url instead for lazy loading
        dpi: Ext.data.Record.create([{'300': 300}]),
        customParams: {
            mapTitle: "Imprimir Mapa",
            comment: "Powered by MapFish"
        }
		});
	
	 this.printButton = new OpenLayers.Control.Button({
         title: "Imprimir Mapa", displayClass: "olControlPrint", trigger: function(){ MapAction.printMap(); }
     });
	 
	 this.zoomBox = new OpenLayers.Control.ZoomBox({ title: "Zoom +" });
  
     zoomToContextExtent = new OpenLayers.Control.Button({
                    title: "Zoom Global", displayClass: "olControlZoomToMaxExtent", trigger: function(){ MapManager.map.zoomToExtent(MapManager.bounds); }
     });              
     
	 this.overViewMap = new OpenLayers.Control.Button({
                    title: "Localizador", displayClass: "olControlOverviewMap", 
					
					trigger: function(){
                		if (MapManager.map.controls[0].visible) {
                    		MapManager.map.controls[0].minimizeControl();
                    		MapManager.map.controls[0].visible = false;
                						}
                		else {
                    		MapManager.map.controls[0].maximizeControl();
                    		MapManager.map.controls[0].visible = true;
                			}
            			}		
                });              
     
	 
     optionsLine = {
                    handlerOptions: {
                        persist: true
                    },
                    displayClass: "olControlMeasureDistance",
                    title: "Medir Distância"
                };

    optionsPolygon = {
                    handlerOptions: {
                        persist: true
                    },
                    displayClass: "olControlMeasureArea",
                    title: "Medir Área"
                };

    measureControls = {
                    line: new OpenLayers.Control.Measure(
                      OpenLayers.Handler.Path, 
                      optionsLine 
                    ),
                    polygon: new OpenLayers.Control.Measure(
                        OpenLayers.Handler.Polygon, 
                        optionsPolygon
                    )
                };
                
    for(var key in measureControls) {
                    control = measureControls[key];
                    control.events.on({
                        "measure": handleMeasurements,
                        "measurepartial": handleMeasurements
                    });
                }   
	
	
    MapManager.featureInfo = new OpenLayers.Control({
        displayClass: "olControlFeatureInfo",
        title: "Identificador"
    });
    
    // register events to the featureInfo tool
    MapManager.featureInfo.events.register("activate", MapManager.featureInfo, function() { MapAction.toggleQueryMode(); });                
    MapManager.featureInfo.events.register("deactivate", MapManager.featureInfo, function() { MapAction.toggleQueryMode(); });
    
    MapManager.showPopUp = new OpenLayers.Control({
        displayClass: "olControlPopUp",
        title: "Cadastrar Objeto"
    });
    
    // register events to the featureInfo tool
    MapManager.showPopUp.events.register("activate", MapManager.showPopUp, function() { MapAction.togglePopUpMode(); });                
    MapManager.showPopUp.events.register("deactivate", MapManager.showPopUp, function() { MapAction.togglePopUpMode(); });
    
    //EDGE MERGER
    MapManager.nodeFinder = new OpenLayers.Control({
        displayClass: "olControlEdgeMerger",
        title: "Cadastrar Edge"
    });
    
    // register events to the featureInfo tool
    MapManager.nodeFinder.events.register("activate", MapManager.nodeFinder, function() { MapAction.toggleNodeFinderMode(); MapAction.openEdgeMergerScreen(); });                
    MapManager.nodeFinder.events.register("deactivate", MapManager.nodeFinder, function() { 
    	
    	MapAction.toggleNodeFinderMode(); 
    
    	
    });
    
    //ROUT MERGER
    MapManager.routMerger = new OpenLayers.Control({
        displayClass: "olControlRoutMerger",
        title: "Cadastrar Rout"
    });
    
    // register events to the featureInfo tool
    MapManager.routMerger.events.register("activate", MapManager.routMerger, function() { MapAction.toggleRoutMergerMode(); MapAction.openRoutMergerScreen(); });                
    MapManager.routMerger.events.register("deactivate", MapManager.routMerger, function() { 
    	
    	MapAction.toggleRoutMergerMode(); 
    
    	
    });
    
 
    // create the panel where the controls will be added
    
   
   	measureControl = new OpenLayers.Control.Measure(
           OpenLayers.Handler.Path, { persist: true }
    );
        
    measureControl.events.on({
            "measure": handleMeasurements,
            "measurepartial": handleMeasurements
    });
            
    function handleMeasurements(event) {
            var geometry = event.geometry;
            var units = event.units;
            var order = event.order;
            var measure = event.measure;
            var element = document.getElementById('output');
            if(order == 1)
            	Ext.getCmp('measure').setText('Distância:'+measure.toFixed(3) + " " + units);
            else
            	Ext.getCmp('measure').setText('Área:'+measure.toFixed(3) + " " + units+"<sup>2</sup>");
    }	
        
	
},

createPanel:function (){
	
	this.initControls();
	
	var dragger = new OpenLayers.Control.DragPan({title:'Arrastar Mapa', displayClass: 'olControlPanMap'});
    
	MapManager.panel = new OpenLayers.Control.Panel({ defaultControl: dragger });
	
	MapManager.panel.addControls([MapManager.featureInfo]);
	
//	if(allowFeature == true)
	MapManager.panel.addControls([MapManager.showPopUp]);
	
	MapManager.panel.addControls([
	                  //  this.overViewMap,
	                    MapManager.routMerger,
	                    MapManager.nodeFinder,
	                    this.box,
	                    this.zoomBox,
	                    dragger,
	 				    zoomToContextExtent,
	                    measureControls.line,
	                    measureControls.polygon
	                    ]); 
	
	if(printCapabilities)
		MapManager.panel.addControls([this.printButton]);

	
}

};