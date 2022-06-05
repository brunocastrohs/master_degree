MapManager = {
		selectLayers:'',
		selectedLayer:'',
		mapPanel:'',
		map:'',
		vector:null,
		clone:null,
		layers: [{title:'',layer:'',url:'',group:'',visibility:'',container:''}],
		containers:[{name:'',radio:true}],
		layersLoaded:'',
		panel:'',
		ovLayers:null,
		featureInfo:'',queryEventHandler:'',popUpEventHandler:'',nodeFinderEventHandler:'',routMergerEventHandler:'',
		layerId:'',showPopUp:'',nodeFinder:'',routMerger:'',LayerNodeUI:null,
		zoom:null,
		center:null,
		projection:null,
		bounds:null,
		gmap:null,
		ghyb:null,
		clusterStore:null,
		googleL:0,
		bairroStore:null,
		bairro:false,
		bairroEmptyText:'Selecione um Bairro...',
		initMap: function(){

			if(this.projection == 'EPSG:900913' && google != null) {
				this.containers.push({name:'GOOGLE',radio:true});
				var options = {
						allOverlayes: true,
						allOverlays:true,
						controls: [],
						maxExtent: this.bounds,
						projection: this.projection,units: 'm'};
				geocoder = new google.maps.Geocoder();
			}
			else if(this.projection == 'EPSG:900913'){
				var options = {
					allOverlays:true,
					controls: [],
					maxExtent: this.bounds,
					projection: this.projection, 
					maxResolution: 2192,
					units: 'm'};
				MapManager.zoom = 7;
				MapManager.googleL = 2;
			}
			else
				var options = {allOverlays:true,controls: [],maxExtent: this.bounds,projection: this.projection,units: 'degrees'};
			
			this.map = new OpenLayers.Map('map', options);

			if(this.projection == 'EPSG:900913' && google != null){
				this.gmap = new OpenLayers.Layer.Google(
						"Google Streets", // the default
						{numZoomLevels: 20, visibility: true,transparent: "TRUE",CONTAINER:'GOOGLE'}
						);
					
				this.ghyb = new OpenLayers.Layer.Google(
						"Google Earth",
						{type: google.maps.MapTypeId.HYBRID, numZoomLevels: 22, visibility: false,transparent: "TRUE",CONTAINER:'GOOGLE'}
						);
			
	        
				this.map.addLayers([this.gmap,this.ghyb]);        
			
			}
			
		    for(i = 0; i != this.layers.length; i++){
				
				this.map.addLayer(this.returnOlLayer(this.layers[i]));
					
			}
			
		    this.vector = new OpenLayers.Layer.Vector("Highlighted Features", {
	            displayInLayerSwitcher: false, 
	            isBaseLayer: false 
		    });
					
		    this.map.addLayers([this.vector]);
	
		},		
		returnOlLayer:function(layer){
			
			return new OpenLayers.Layer.WMS(
					layer['title'], 
					layer['url'],
					{'layers':layer['name'],transparent: true, format: 'image/png'},
					{transitionEffect:'resize', displayInLayerSwitcher: layer['layerswitcher'] , VIEW:layer['view'], GROUP:layer['group'],CONSULTABLE:layer['consultable'],SCHEMA:layer['schema'],CONTAINER:layer['container'],visibility:layer['visibility'], singleTile: true}
					);
			
		},
		generateLayersPanels: function(){
			
			var j,i = 0;
			
			var olStoreLayers = [];
			
			var olLayersPanels = [];
			
		//	if(this.projection == 'EPSG:900913' && google != null)
		//		olLayersPanels.push(this.returnContainer('GOOGLE',null));
			
			for(i = 0; i != this.map.layers.length; i++)
				olStoreLayers.push(this.map.layers[i]);
			
			for(j = 0; j != this.containers.length; j++){
				olLayersPanels.push(this.returnTreePanel(this.containers[j]['name'], this.returnContainer(this.containers[j],olStoreLayers)));
			}
			
			return olLayersPanels;
			
		},
		returnContainer:function(container,layers){
			
			if(container['radio'])
				return new GeoExt.tree.LayerContainer({text: container['name'],expanded: true, layerStore: new GeoExt.data.LayerStore({ layers: layers}), loader: { baseAttrs: {radioGroup: 'over',uiProvider: "layernodeui"} }  } );
			else if(container == 'GOOGLE')
				return new GeoExt.tree.LayerContainer({text: 'Google',layerStore: new GeoExt.data.LayerStore({ layers: [this.gmap,this.ghyb]}),leaf: false,expanded: true} );
			else
				return new GeoExt.tree.LayerContainer({text: container['name'],layerStore: new GeoExt.data.LayerStore({ layers: layers}),leaf: false,expanded: true} );
			
		},
		returnTreePanel:function(name,container){
			
			var layerOpacity = new GeoExt.LayerOpacitySlider({
				aggressive : true,
				width : 100,
				inverse : true
			// renderTo: "transparencia"
			});

			
			return new Ext.tree.TreePanel({
				title: name,
				//iconCls: 'layerTree',
			    autoScroll: true,
			    collapseMode : 'mini',
				collapsible : true,
				animate : true,
		        loader: new Ext.tree.TreeLoader({
	                  applyLoader: false,
	                  uiProviders: {
	                	  "ui": GeoExt.ivone.LayerNodeUI,
	                      "layernodeui": this.LayerNodeUI
	                  }
		        }),
		        plugins: [{
		            ptype: "gx_treenodeactions",
		            listeners: {
		                action: GeoExt.ivone.onAction
		            }
		        },
		        new GeoExt.plugins.TreeNodeRadioButton({
                      listeners: {
                          "radiochange": function(node) {
                        	  MapManager.layerId = node['attributes']['layer']['id'];
                        	  layerOpacity.setLayer(node.layer);
                          }
                      }
                  })
		        ],
		       root: {
		    	   children:container,
		    	   nodeType: "gx_layercontainer",
		    	   loader: {
		    		   	filter: function(record) {
		    			   	var layer = record.get("layer");
		    			    //return (!layer.isBaseLayer && layer.name == "2000 Census - Population (TC)");
		    			   	return layer.options.CONTAINER == name;
		    			},
		                baseAttrs: {
		                    radioGroup: "radiogroup",
		                    uiProvider: "ui",
		                    actions: [{action: "number",qtip: "number"}]
		                }
		            }
		        
		    	},
		       lines: false,
		       rootVisible: false,
		       tbar : ["Transparência :",layerOpacity]
		    });
			
			
		},
		returnClusterForm:function(){
			
			var ufStore = new Ext.data.ArrayStore({
		        fields: ['nome', 'gid'],
		        data : [['CE','1']] 
		    });
			
			var municipioStore = new Ext.data.ArrayStore({
		        fields: ['nome', 'gid'],
		        data : [['Fortaleza','1']] 
		    });
			
			return new Ext.FormPanel({
		        labelWidth: 75, // label settings here cascade unless overridden
		       // url:'save-form.php',
		        frame:true,
		        bodyStyle:'padding:5px 5px 0',
		       // width: 350,
		        region:'north',
		        items: [{
		            xtype:'fieldset',
		            title: 'Alterar Visualização de Clientes',
		            collapsible: true,
		            collapsed: false,
		            autoHeight:true,
		            defaultType: 'textfield',
		            items :[
		                    
		                    new Ext.form.ComboBox({
		                    	id:'uf',
		                    	store: ufStore,
		                    	fieldLabel: 'UF',
		                    	displayField:'nome',
		                    	typeAhead: true,
		                    	mode: 'local',
		                    	width: 210,
		                    	//	forceSelection: true,
		                    	triggerAction: 'all',
		                    	emptyText:'CE',
		                    	selectOnFocus:true,
		                    }),
		                    new Ext.form.ComboBox({
		                    	id:'municipio',
		                    	store: municipioStore,
		                    	fieldLabel: 'Munícipio',
		                    	displayField:'nome',
		                    	typeAhead: true,
		                    	width: 210,
		                    	mode: 'local',
		                    	triggerAction: 'all',
		                    	emptyText:'Fortaleza',
		                    	selectOnFocus:true,
		                    }),
		                    
		                 new Ext.form.ComboBox({
		                	id:'bairro',
		    		        store: MapManager.bairroStore,
		    		        fieldLabel: 'Bairro',
		    		        displayField:'nome',
		    		        valueField:'gid',
		    		        typeAhead: true,
		    		        mode: 'local',
		    		        width: 210,
		    		        //forceSelection: true,
		    		        triggerAction: 'all',
		    		        emptyText:MapManager.bairroEmptyText,
		    		        selectOnFocus:true,
		    		        listeners:{
		    		            'select': function(record){
		    		            	MapManager['bairro'] = record.value;
		    		            }
		    		       }
		    		    }),
		    		    {
		                    fieldLabel: 'Limite',
		                //    name: 'home',
		                    width: 50,
		                    id:'limitFilter',
		                    value: ''
		                },
		                new Ext.Button({
		                    text: 'Atualizar',
		                    width: 80,
		                 //   iconCls : 'byNumber',
		                    style: 'float:right',
		                    handler: MapAction.updateViewNumber
		                })
		            ]
		            
		        },
		        {
		            xtype:'fieldset',
		            title: 'Setorização por Quantidade',
		            collapsible: true,
		            collapsed: true,
		            autoHeight:true,
		            defaultType: 'textfield',
		            items :[{
		                    fieldLabel: 'Quantidade',
		                //    name: 'home',
		                    width: 210,
		                    id:'numberFilter',
		                    value: '5'
		                },
		                new Ext.Button({
		                    text: 'Setorizar',
		                    width: 100,
		                 //   iconCls : 'byNumber',
		                    style: 'float:right',
		                    handler: MapAction.kumeByNumber
		                })
		            ]
		            
		        },
		        {
		            xtype:'fieldset',
		            title: 'Setorização por Distância',
		            collapsible: true,
		            collapsed: true,
		            autoHeight:true,
		            defaultType: 'textfield',
		            labelWidth:90,
		            items :[{
		                    fieldLabel: "Distância(km's)",
		                   // name: 'home',
		                    width: 205,
		                    id:'distanceFilter',
		                    value: '15'
		                },
		                new Ext.Button({
		                    text: 'Setorizar',
		                  //  iconCls : 'byDistance',
		                    width: 100,
		                    style: 'float:right',
		                    handler: MapAction.kumeByDistance
		                })
		            ]
		            
		        },
		        {
		            xtype:'fieldset',
		            title: 'Setorização por Tempo',
		            collapsible: true,
		            collapsed: true,
		            autoHeight:true,
		            defaultType: 'textfield',
		            labelWidth:210,
		            items :[{
		                    fieldLabel: "Tempo(min's)",
		              //      name: 'home',
		                    width: 80,
		                    id:'timeFilter',
		                    value: '220'
		                },
		                {
		                    fieldLabel: "Tempo Médio de Atendimento(min's)",
		              //      name: 'home',
		                    width: 80,
		             //       labelWidth:110,
		                    id:'xA',
		                    value: '15'
		                },
		                {
		                    fieldLabel: "Velocidade Média(km/h)",
		              //      name: 'home',
		                    width: 80,
		                 //   labelWidth:110,
		                    id:'xV',
		                    value: '20'
		                },
		                new Ext.Button({
		                    text: 'Setorizar',
		                    //iconCls : 'byTime',
		                    width: 100,
		                    labelWidth:110,
		                    style: 'float:right',
		                    handler: MapAction.kumeByTime
		                })
		            ]
		            
		        }		        
		        ],
		        buttons: ['Legenda dos Botões',{iconCls : 'helpButton',handler : MapAction.openLegendWindow},
		                  'Mostrar todos os Setores',{iconCls : 'gridUpdate',handler : MapAction.restoreCluster}
		                  ]
		    });
			
		},
		returnSubClusterForm:function(kume){
			
			data = new Date();
						
			var today = data.getDate()+"/"+ (data.getMonth()+1)+"/"+data.getFullYear();
			
			return new Ext.FormPanel({
		        labelWidth: 75, // label settings here cascade unless overridden
		       // url:'save-form.php',
		        frame:true,
		        bodyStyle:'padding:5px 5px 0',
		       // width: 350,
		        region:'north',
		        items: [{
		            xtype:'fieldset',
		            title: 'Setorização por Quantidade',
		            collapsible: true,
		            collapsed: false,
		            autoHeight:true,
		            defaultType: 'textfield',
		            items :[{
		                    fieldLabel: 'Quantidade',
		                //    name: 'home',
		                    width: 210,
		                    id:'subNumberFilter',
		                    value: '5'
		                },
		                new Ext.Button({
		                    text: 'Setorizar',
		                    width: 100,
		                 //   iconCls : 'byNumber',
		                    style: 'float:right',
		                    handler: function(){MapAction.subKumeByNumber(kume);}
		                })
		            ]
		            
		        },
		        {
		            xtype:'fieldset',
		            title: 'Setorização por Distância',
		            collapsible: true,
		            collapsed: true,
		            autoHeight:true,
		            defaultType: 'textfield',
		            labelWidth:90,
		            items :[{
		                    fieldLabel: "Distância(km's)",
		                   // name: 'home',
		                    width: 205,
		                    id:'subDistanceFilter',
		                    value: '15'
		                },
		                new Ext.Button({
		                    text: 'Setorizar',
		                  //  iconCls : 'byDistance',
		                    width: 100,
		                    style: 'float:right',
		                    handler: function(){MapAction.subKumeByDistance(kume);}
		                })
		            ]
		            
		        },
		        {
		            xtype:'fieldset',
		            title: 'Setorização por Tempo',
		            collapsible: true,
		            collapsed: true,
		            autoHeight:true,
		            defaultType: 'textfield',
		            labelWidth:210,
		            items :[{
		                    fieldLabel: "Tempo(min's)",
		              //      name: 'home',
		                    width: 70,
		                    id:'subTimeFilter',
		                    value: '220'
		                },
		                {
		                    fieldLabel: "Tempo Médio de Atendimento(min's)",
		              //      name: 'home',
		                    width: 70,
		                    id:'subXA',
		                    value: '15'
		                },
		                {
		                    fieldLabel: "Velocidade Média(km/h)",
		              //      name: 'home',
		                    width: 70,
		                    id:'subXV',
		                    value: '20'
		                },
		                new Ext.Button({
		                    text: 'Setorizar',
		                    //iconCls : 'byTime',
		                    width: 100,
		                    style: 'float:right',
		                    handler: function(){MapAction.subKumeByTime(kume);}
		                })
		            ]
		            
		        },
		        
		        ],
		        buttons: ['Dia Inicial:',
				new Ext.form.DateField({id: 'doi',hidden:false,emptyText:today,format:  'd/m/Y',width:100,allowBlank:true}),
		                  'Mostrar todos os Setores',{iconCls : 'gridUpdate',handler : function(){MapAction.restoreSubCluster(kume);}}]
		    });
			
		},
		initGIS:function(){
			
			//new OpenLayers.LonLat(-109.6, 46.7).transform("EPSG:900913","EPSG:4326");
			
			this.initMap();	
			
			this.LayerNodeUI = Ext.extend(GeoExt.tree.LayerNodeUI, new GeoExt.tree.TreeNodeUIEventMixin());
			
			Ext.namespace("GeoExt.ivone");

			GeoExt.ivone.onAction = function(node, action, evt) {
			    var layer = node.layer;
			    switch(action) {
			        case "number":
			        	
			        if(layer['SCHEMA'])	
			        	ActionManager.sendNumberRequest({title:layer.name,'x':layer.SCHEMA,'y':layer.params.LAYERS});
			        
			        break;
			    }
			};

			// custom layer node UI class
			GeoExt.ivone.LayerNodeUI = Ext.extend(
			    GeoExt.tree.LayerNodeUI,
			    new GeoExt.tree.TreeNodeUIEventMixin()
			);
			
			//var olContainers = this.generateContainers();
			
			this.map.addControl(new OpenLayers.Control.PanZoomBar());
			this.map.addControl(new OpenLayers.Control.ScaleLine());
			//this.map.addControl(new OpenLayers.Control.Navigation({documentDrag: true}));
			//this.map.addControl(new OpenLayers.Control.LayerSwitcher({'ascending':false}));
			
			
		    this.map.events.register('mousemove', this.map, function(e){
		        
		    	var lonLat = MapManager.map.getLonLatFromPixel(e.xy);
		    	
		    	if(MapManager.projection == 'EPSG:900913')
		    		lonLat = lonLat.transform(new OpenLayers.Projection("EPSG:900913"),new OpenLayers.Projection("EPSG:4326"));
		           
		        Ext.getCmp('mousePosition').setText('Lon: ' + lonLat.lon+' , Lat: ' + lonLat.lat);
		        
		    });
		    
		    this.mapPanel = new GeoExt.MapPanel({
		        region: 'center',
		        zoom: this.zoom,
		        center:this.center,
		        map: this.map,
		        width: 100,
		        height: 100,
		        layers: this.map.layers,
		    });
		    
		    var north = new Ext.Toolbar({region:'north'});
			 
			north.add(MenuManager.logo);
			
			var layerOpacity = new GeoExt.LayerOpacitySlider({
				aggressive : true,
				width : 90,
				inverse : true
			// renderTo: "transparencia"
			});

			var treePanels = this.generateLayersPanels();
						
			var clusterForm = MapManager.returnClusterForm();
			
			var clusterResult = {autoScroll: true,region: 'center',border: true, id:'clusterResult'};
			
			var east = {title: 'Logística',width:350,autoScroll: true ,region: 'east',split: true,collapseMode: 'mini',border: true, iconCls: 'clusterForm',items: [clusterForm,clusterResult]};
			
			var filterTab = {title: 'Filtro',height:(window.innerHeight),autoScroll: true,autoLoad: {url:'/ARGO/pages/Common/QueryGISForm.php',scripts:false} ,border: true, iconCls: 'search'};
			
			var geoCodeTab = {title: 'Geocod.',height:(window.innerHeight),autoScroll: true,autoLoad: {url:'/ARGO/pages/Common/GeocodForm.php',scripts:false} ,border: true, iconCls: 'search'};

			var legendPanel = new GeoExt.LegendPanel({title: 'Legendas',autoScroll: true,border: true,height:(window.innerHeight),iconCls: 'legends'});
			
			var layerPanel = new Ext.Panel({
				title : "Camadas",
				animate : true,
				autoScroll : true,
				enableDD : true,
				iconCls: 'layerTree',
				items : treePanels
			});
			
			var tabs = new Ext.TabPanel({
				border:true,
				width: 350,
		        region: 'west',
		        activeTab: 0,
		        tabPosition: 'top',
		        items: [layerPanel, legendPanel, filterTab,geoCodeTab]
		    });
			
			var west = new Ext.Panel({
				border:true,
				width:'350',
				region:'west',
				split:true,
				collapsible:true,
				collapsed:false,
				items:[north,tabs]
			}) 
			
			MapControls.createPanel();
                
			this.map.addControl(this.panel);
			
			var center = new Ext.Panel({
		        region: 'center',
		        layout: 'border',
		        items: [this.mapPanel,west,
		                east
		                ],
		        bbar: new Ext.Toolbar({
		            items: [
		            {iconCls:'scrollReportItem',report:'sequencia',text : 'Sequência de Visita',handler : MenuManager.onReportButtonClick},
		            MenuManager.separator,
		    		{iconCls:'scrollReportItem',report:'rota',text : 'Rota',handler : MenuManager.onReportButtonClick},
		    		MenuManager.separator,
		    		{id: 'measure',text: 'Distância/Área:'}, 
		            '->', 
		            {id: 'mousePosition',text: 'Lat: 0, Lon: 0'}
		            ]
		        })
		    });
			
			/*MenuManager.north = new Ext.Toolbar({region:'north',height:50});
			
		    MenuManager.north.add(MenuManager.logo//, 
		    		//MenuManager.mover,
					//'Usuário:', new Ext.form.TextField({id: 'login'}), MenuManager.separator, 
					//'Senha:', new Ext.form.TextField({id: 'pass',inputType:'password'}), MenuManager.separator, 
					//{text : 'Login',iconCls : 'menuItemLogin',handler : MenuManager.onLoginButtonClick}

			);*/
			
		    MapManager.queryEventHandler = new OpenLayers.Handler.Click({ 'map': this.map }, { 'click': function(e) { MapAction.doGetFeatureInfo(e); } });
		    
		    MapManager.popUpEventHandler = new OpenLayers.Handler.Click({ 'map': this.map }, { 'click': function(e) { MapAction.openPopUp(e); } });		    
		    
		    MapManager.nodeFinderEventHandler = new OpenLayers.Handler.Click({ 'map': this.map }, { 'click': function(e) { MapAction.doNodeFinder(e); } });
		    
		    MapManager.routMergerEventHandler = new OpenLayers.Handler.Click({ 'map': this.map }, { 'click': function(e) { MapAction.doRoutMerger(e); } });
		   
		    Ext.Ajax.timeout = 9000000;  
			
			Ext.Ajax.on('beforerequest', ActionManager.showSpinner, this);
			Ext.Ajax.on('requestcomplete', ActionManager.hiddeSpinner, this);
			Ext.Ajax.on('requestexception', ActionManager.hiddeSpinner, this);

		    
		    new Ext.Viewport({
		        layout: 'border',
		        items: [center]
		    });
		    
	}


};