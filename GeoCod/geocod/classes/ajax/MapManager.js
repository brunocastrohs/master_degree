MapManager = {
		selectLayers:'',
		selectedLayer:'',
		mapPanel:'',
		map:'',
		clone:null,
		layers: [{title:'',layer:'',url:'',group:'',visibility:'',container:''}],
		containers:[{name:'',radio:true}],
		layersLoaded:'',
		panel:'',
		ovLayers:null,
		featureInfo:'',queryEventHandler:'',popUpEventHandler:'',layerId:'',showPopUp:'',LayerNodeUI:null,
		bounds:new OpenLayers.Bounds(-41.4131257835556, -7.85442664589072,-37.2646826043524, -2.80045422498318),
		zoom:2,
		center:[-39.402627736, -4.994973267],
		projection:"EPSG:4326",
		gmap:null,
		ghyb:null,
		initMap: function(){

			var options = {
					allOverlays:true,controls: [],
					//maxExtent: this.bounds,
					maxResolution: 0.0197420797691701,
					projection: this.projection,
					units: 'degrees'
						};
			
			this.map = new OpenLayers.Map('map', options);

			layer = new OpenLayers.Layer.WMS( "OpenLayers WMS","http://vmap0.tiles.osgeo.org/wms/vmap0", {layers: 'basic'} );

			untiled = new OpenLayers.Layer.WMS("Ruas","http://localhost:8080/geoserver/geocod/wms", 
					{LAYERS : 'geocod:via',STYLES : '',format : 'image/png',transparent:true}, 
					{transparent:true,singleTile : true,ratio : 1}
				);

			this.vector = new OpenLayers.Layer.Vector("vector");

			this.map.addLayers([ layer,untiled,this.vector]);
			
		},
		generateContainers: function(){
			
			var olStoreLayers = [];
			
			var olContainer = [];
			
			var j,i = 0;
			
			olContainer.push(this.returnContainer('GeoCod',null));
			
			return olContainer;
		},
		returnContainer:function(container,layers){
			
				return new GeoExt.tree.LayerContainer({text: 'GeoCod',layerStore: new GeoExt.data.LayerStore({ layers: this.map.layers}),leaf: false} );
				
			
		},
		initGIS:function(){
			
				this.initMap();	
			
				this.LayerNodeUI = Ext.extend(GeoExt.tree.LayerNodeUI, new GeoExt.tree.TreeNodeUIEventMixin());
				
				var olContainers = this.generateContainers();
				
				this.map.addControl(new OpenLayers.Control.PanZoomBar());
				this.map.addControl(new OpenLayers.Control.ScaleLine());
				this.map.addControl(new OpenLayers.Control.Navigation({documentDrag: true}));
				
			    this.map.events.register('mousemove', this.map, function(e){
			        var lonLat = MapManager.map.getLonLatFromPixel(e.xy);
			        Ext.getCmp('mousePosition').setText('Lon: ' + lonLat.lon+' , Lat: ' + lonLat.lat);
			    });
			    
			    this.mapPanel = new GeoExt.MapPanel({
			        region: 'center',
			        zoom: this.zoom,
			        center:this.center,
			        map: this.map,
			        width: 100,
			        height: 100
			    });
			    
			    var north = new Ext.Toolbar({region:'north',height:50});
				
			    var store = new Ext.data.SimpleStore({id: 0,fields: ['gid','nome'],data: [['97','Caucaia'],['181','Fortaleza'],['102','Aquiraz']]}); 
			    
				north.add(
						MenuManager.logo,
					//	new Ext.form.ComboBox({store: store,style:'margin-left:45%;',displayField: 'nome',valueField: 'gid',typeAhead: true,mode: 'local',triggerAction: 'all',emptyText: 'Cidade...',selectOnFocus: true,iconCls: 'no-icon'}),
						new Ext.form.TextField({id: 'filter',style:'margin-left:15%;',width:700}), 
						MenuManager.separator,
						{iconCls : 'search',handler : MapAction.enableFilter},
						{iconCls : 'phoneSearch',handler : MapAction.enablePhoneFilter},
						{iconCls : 'levenshtainSearch',handler : MapAction.enableLevenshtainFilter},
						{iconCls : 'search',handler : function(){
								window.alert(MapManager.map.getLonLatFromViewPortPx());
							}
						}

				);

				//getLonLatFromViewPortPx
			    
				var searchForm = {title: 'String Resultados',autoScroll: true,border: true, iconCls: 'search',id:'records'};

				var phoneSearchForm = {title: 'Phonema Resultados',autoScroll: true,border: true, iconCls: 'phoneSearch',id:'phoneRecords'};
				
				var levenshtainSearchForm = {title: 'Levenshtain Resultados',autoScroll: true,border: true, iconCls: 'levenshtainSearch',id:'levenshtainRecords'};
				
				var west = new Ext.TabPanel({
			        border:true,
					width: 500,
			        region: 'east',
					split: true,
			        collapsible: true,
			        autoScroll: true,
			     //   collapsed:true,
			        //activeTab: 0,
			        tabPosition: 'bottom',
			        items: [searchForm,searchForm,phoneSearchForm,levenshtainSearchForm]
			    });
				
				var center = new Ext.Panel({
			        region: 'center',
			        layout: 'border',
			        items: [this.mapPanel, west],
			        bbar: new Ext.Toolbar({
			            items: [{
							id: 'measure',
			                text: 'Distância/Área:'
			            }, '->', {
			                id: 'mousePosition',
			                text: 'Lat: 0, Lon: 0'
			            }]
			        })
			    });
				
			    new Ext.Viewport({
			        layout: 'border',
			        items: [north,center]
			    });
				
		}

};