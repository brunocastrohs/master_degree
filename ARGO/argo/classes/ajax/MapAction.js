MapAction = {
		
    realm: 'ARGO',	
	
	// PRINT

	printMap : function() {
		// A window with the PrintMapPanel, which we can use to adjust
		// the print extent before creating the pdf.
		printDialog = new Ext.Window(
				{
					title : "Print Preview",
					layout : "fit",
					width : 350,
					autoHeight : true,
					items : [ {
						xtype : "gx_printmappanel",
						sourceMap : MapManager.mapPanel,
						printProvider : MapControls.printProvider
					} ],
					bbar : [
							{
								xtype : 'textfield',
								name : 'field1',
								id : 'print_title',
								emptyText : 'Title'
							},
							'-',
							{
								xtype : 'textfield',
								name : 'field2',
								id : 'print_comment',
								emptyText : 'Comment'
							},
							'-',
							{
								text : "Criar PDF",
								handler : function() {
									
									MapControls.printProvider.dpi.data = {name:'300',value:300};

									var title = document.getElementById('print_title').value;

									if (title)
										MapControls.printProvider.customParams['mapTitle'] = title;

									var comment = document.getElementById('print_comment').value;

									if (comment)
										MapControls.printProvider.customParams['comment'] = comment;

									printDialog.items.get(0).print();

								}
							} ]
				});
		printDialog.show();
	},

	// PopUp

	openPopUp : function(evt) {

		var popup;

		if (evt) {

			var loc = MapManager.map.getLonLatFromPixel(evt.xy);

			var activeLayer = MapManager.map.getLayer(MapManager.layerId);
			
			if (activeLayer != null) {
				if (activeLayer.options.GROUP == 'point') {

				var stg = activeLayer.params['LAYERS'];

				var href = "'http://localhost:80/ARGO/classes/receiver/GenericReceiver.php?action=gisInsertForm&x="+activeLayer.options.SCHEMA;

				var lonLat = MapManager.map.getLonLatFromPixel(evt.xy);

				var html = '<iframe style="width:100%;height:300px;padding:0;" frameborder="0" id="feature" src='
						+ href
						+ '&y='+stg
						+ '&lat='+lonLat.lat
						+ '&long='+lonLat.lon
						+ "'></iframe>";
				
				if (!popup) {
					popup = new GeoExt.Popup({
						title : activeLayer.name,
						width : 500,
						maximizable : true,
						collapsible : true,
						map : MapManager.mapPanel.map,
						anchored : true,
						listeners : {
							close : function() {
								MapManager.map.getLayer(MapManager.layerId).redraw(true);
								popup = null;
							}
						}
					});
				}

				popup.add({
					xtype : "box",
					autoEl : {
						html : html
					}
				});

				// reset the popup's location
				popup.location = loc;

				popup.doLayout();

				popup.show();

			}
			}else
				Ext.Msg.show({title : 'Aviso',msg : 'É preciso ativar uma camada!',icon : Ext.MessageBox.WARNING});

		} 
	},

	togglePopUpMode : function() {
		if (MapManager.showPopUp.active) {
			MapManager.popUpEventHandler.activate();
		} else {
			MapManager.popUpEventHandler.deactivate();
		}
	},

	doGetFeatureInfo : function(evt) {
		var popup;

		if (evt) {

			var loc = MapManager.map.getLonLatFromPixel(evt.xy);

			var activeLayer = MapManager.map.getLayer(MapManager.layerId);

			if (activeLayer != null) {
				var url = activeLayer.getFullRequestString({
					REQUEST : "GetFeatureInfo",
					EXCEPTIONS : "application/vnd.ogc.se_xml",
					BBOX : activeLayer.map.getExtent().toBBOX(),
					X : evt.xy.x,
					Y : evt.xy.y,
					INFO_FORMAT : 'text/html',
					QUERY_LAYERS : activeLayer.params.LAYERS,
					WIDTH : activeLayer.map.size.w,
					HEIGHT : activeLayer.map.size.h
				});

			var width = 500;
				
			if (!popup) {
					popup = new GeoExt.Popup({
						title : activeLayer.name,
						width : 500,
						maximizable : true,
						collapsible : true,
						map : MapManager.mapPanel.map,
						anchored : true,
						listeners : {
							close : function() {
								if(MapManager.map.getLayer(MapManager.layerId).options.GROUP == 'point')
									MapManager.map.getLayer(MapManager.layerId).redraw(true);
								popup = null;
							}
						}
					});
				}

			
				var view = activeLayer.options.VIEW;
				
				var html = '<iframe style="width:100%;height:350px;padding:0;" frameborder="0" id="feature" src="'+ url +'&realm='+MapAction.realm+'&k='+activeLayer.options.SCHEMA+'&view='+view+'">';
				
				// add some content to the popup (this can be any Ext component)
				popup.add({
					xtype : "box",
					autoEl : {
						html : html
					}
				});

				// reset the popup's location
				popup.location = loc;

				popup.doLayout();

				popup.show();

			}
			else
				Ext.Msg.show({title : 'Aviso',msg : 'É preciso ativar uma camada!',icon : Ext.MessageBox.WARNING});

		} 

	},
	
	toggleQueryMode : function() {
		if (MapManager.featureInfo.active) {
			MapManager.queryEventHandler.activate();
		} else {
			MapManager.queryEventHandler.deactivate();
		}
	},

	// Filter Hour
	oneClick : false,
	width:0,
	layerId:null,
	attCount : 0,
	
	filter : {
		x:'',
		y : "",
		params : "",
		nome : ""
	},

	populateLayerSelector : function() {

		if (!this.oneClick) {

			document.getElementById('layerSelector').innerHTML = '<OPTION value=""></OPTION>';

			for (j = 0; j != MapManager.map.layers.length; j++) {
				if (MapManager.map.layers[j].options.CONSULTABLE)
					if (MapManager.map.layers[j].params)
						document.getElementById('layerSelector').innerHTML += '<OPTION value="'+ MapManager.map.layers[j].id+ '">'+ MapManager.map.layers[j].name + '</OPTION>';
			}

			this.oneClick = true;

		}

	},

	populateLayerAttributes : function() {

		var layerId = document.getElementById('layerSelector').value;

		if (layerId != "") {

			var layer = MapManager.map.getLayer(layerId);

			var layerName = layer.params['LAYERS'];

			var object = {
				"x":layer.options.SCHEMA,
				"y" : layerName.replace('graph:', '')
			};

			GenericBean.retrieveGISColumns(object, {
				preloader : "pr",
				target : 'layerAttributes0',
				target_attr : 'innerHTML'
			});
			
			this.width = (document.getElementById('layerAttributes0').options.length-1) * 100;
			
			if(this.width > 1000)
				this.width = 1000;
			
		} else if(document.getElementById('layerAttributes'))
			document.getElementById('layerAttributes').innerHTML = "";
		
	},

	layerAttributesChange : function(index) {

		var selectedOption = document.getElementById('layerAttributes' + index);

		var layerType = selectedOption.options[selectedOption.selectedIndex].id;

		document.getElementById('sample' + index).innerHTML = layerType;

	},

	isInteger : function(index, campo) {

		var selectedOption = document.getElementById('layerAttributes' + index);

		var layerType = selectedOption.options[selectedOption.selectedIndex].id;

		if (layerType == "integer" || layerType == "double precision") {

			var digits = "0123456789.";
			var campo_temp;
			for ( var i = 0; i < campo.value.length; i++) {
				campo_temp = campo.value.substring(i, i + 1);
				if (digits.indexOf(campo_temp) == -1) {
					campo.value = campo.value.substring(0, i);
				}
			}
		}
	},
	
	autoComplete : function(index,value) {
		
		var selectedOption = index == '1000'?{value:'address'}:document.getElementById('layerAttributes'+ index);
		
		var layerType = document.getElementById('sample'+ index).innerHTML;
		
		//document.getElementById('autocomplete'+index).innerHTML = "<span class='at' id='at0'><img src='/ARGO/images/autocomplete.gif' alt='loading' /></span>";
		
		if(value.length > 1 && (selectedOption != null && selectedOption.value != '')){

			if( layerType.search("text") != -1 | layerType.search("character") != -1 ){
			
				var layerName = index == '1000'?'cliente_n':new String(MapManager.clone.params['LAYERS']);
				
				layerName = layerName.replace("graph:", "");
			
				var schema = index == '1000'?'public':MapManager.clone.options.SCHEMA;
				
				var post = {
					y : schema,
					x : layerName,
					k : selectedOption.value,
					index : index,
					param:selectedOption.value+" like '%"+value+"%'",
					action : 'autocomplete',
				};

				page = '/ARGO/classes/receiver/GenericReceiver.php';

				PLX.Request({
					url : page,
					method : "post",
					target : 'autocomplete'+index,
					preloader : 'at'+index,
					params : post
				});
		
			}
		}
		document.getElementById('autocomplete'+index).innerHTML = "";
		
	},
	
	getAutoValue : function(index,value) {
			
		document.getElementById('filterValueField'+index).value = value;
		
		document.getElementById('autocomplete'+index).innerHTML = '';
		
	},

	createAttribute : function(index) {

		var firstContent = document.getElementById('layerAttributes0').innerHTML;

		return '<div id="attribute' + index	+ '"><div class="label">Atributos <select id="layerAttributes'+ index
				+ '" class="layerAttributes" onchange="MapAction.layerAttributesChange('+ index + ');">' + firstContent + '</select></div>'
				+ '<div class="label">Valor <input id="filterValueField'+ index	+ '" class="filterValueField" type="text" onkeyup="MapAction.isInteger('
				+ index + ',this);MapAction.autoComplete('+ index + ',this.value);" /></div><div id="autocomplete' + index+ '" class="autocomplete"></div><span class="at" id="at' + index+ '"><img src="/ARGO/images/autocomplete.gif" alt="loading" /></span>'
				+'<div id="sample' + index+ '" class="sample"></div></div>';

		
		//MapAction.autoComplete('+ index + ',this.value);
	},

	addAttribute : function() {

		var ts = document.getElementById('layerAttributes0').textContent;

		if (ts != "") {
			this.attCount++;

			if (this.attCount == 1)
				document.getElementById('addedAttributeList').innerHTML += '<div id="dropButton" onclick="MapAction.dropAttribute();">Remover Atributo</div>';

			document.getElementById('addedAttributeList').innerHTML += this.createAttribute(this.attCount);
		}
	},

	dropAttribute : function() {

		if (this.attCount > 0) {
			this.attCount--;

			var list = document.getElementById('addedAttributeList');

			list
					.removeChild(document.getElementById('addedAttributeList').lastChild);

			if (this.attCount == 0)
				list
						.removeChild(document
								.getElementById('addedAttributeList').firstChild);
		}

	},

	layerChange : function(id) {

		if (MapManager.clone != "" && MapManager.clone != null) {
			this.disableFilter();
		}

		if(id)
		this.createClone(id);

		this.attCount = 0;

		document.getElementById('addedAttributeList').innerHTML = "";

	},

	createClone : function(id) {

		this.layerId = id;
		
		MapManager.clone = MapManager.map.getLayer(id).clone();
		
		MapManager.clone.name = 'Pesquisa';

		if (MapManager.clone.options.GROUP == 'point') {
			MapManager.clone.mergeNewParams({
				styles : 'point'
			});
		}

		if (MapManager.clone.options.GROUP == 'polygon') {
			MapManager.clone.mergeNewParams({
				styles : 'polygon'
			});
		}

		if (MapManager.clone.options.GROUP == 'line') {
			MapManager.clone.mergeNewParams({
				styles : 'line'
			});
		}

		MapManager.clone.setVisibility(false);

		MapManager.map.addLayer(MapManager.clone);

	},

	enableFilter : function() {
		if (MapManager.clone != null) {

			var filterParams = {
				cql_filter : ""
			};
			
			for (i = 0; i != this.attCount + 1; i++) {

				var selectedOption = document.getElementById('layerAttributes'+ i);

				if (selectedOption != null && selectedOption.value != '') {

					var layerType = selectedOption.options[selectedOption.selectedIndex].id;

					var key = document.getElementById('filterValueField'+ i).value;
					

					var harry = layerType.search("text");
					
					var junior = layerType.search("character");
					
					if (i > 0) {
						if (layerType.search("text") != -1 | layerType.search("character") != -1 )
							filterParams["cql_filter"] += " OR " + selectedOption.value + " like '%" + key + "%'";
						else{
							filterParams["cql_filter"] += " OR " + selectedOption.value + " = " + key;
						}
					} else {
						if (layerType.search("text") != -1 | layerType.search("character") != -1)
							filterParams["cql_filter"] += selectedOption.value + " like '%" + key + "%'";
						else{
							filterParams["cql_filter"] += selectedOption.value + " = " + key;
						}
					}
				} else {
					return false;
				}
			}
			
			if(!document.getElementById('applyFilterBox').checked){
				this.mergeNewParams(filterParams);
				
			}else{
				this.mergeNewParams(filterParams);
				MapManager.clone.setVisibility(true);
			}

			var layerName = new String(MapManager.clone.params['LAYERS']);

			layerName = layerName.replace("graph:", "");

			var cqlFilter = filterParams["cql_filter"];

			this.filter = {
				x:MapManager.clone.options.SCHEMA,
				y : layerName,
				query : cqlFilter,
				nome : MapManager.clone.name
			};

			MapManager.vector.removeAllFeatures();
			
			ActionManager.sendGISRequest(this.filter, 'querygis', 'records');

		}
	},
	
	retrieveRecords : function() {

		var content = document.getElementById('records').innerHTML;

		var number = document.getElementById('number').innerHTML;
		
		var win = new Ext.Window({
			title : this.filter['nome']+": Econtrados "+number+" registros.",
			width : 500,
			height:400,
			autoScroll : false,
			items : [ {
				html : content
			} ]
		});
	
		win.show();

	},
	
	openColumnScreen : function() {
		
		if (MapManager.clone != null) {

			var filterParams = {
				cql_filter : ""
			};

			for (i = 0; i != this.attCount + 1; i++) {

				var selectedOption = document.getElementById('layerAttributes'+ i);

				if (selectedOption != null && selectedOption.value != '') {

					var layerType = selectedOption.options[selectedOption.selectedIndex].id;

					var key = document.getElementById('filterValueField'+ i).value;
					
					if (i > 0) {
						if (layerType.search("text") != -1 | layerType.search("character") != -1 )
							filterParams["cql_filter"] += " OR " + selectedOption.value + " = '" + key + "'";
						else{
							filterParams["cql_filter"] += " OR " + selectedOption.value + " = " + key;
						}
					} else {
						if (layerType.search("text") != -1 | layerType.search("character") != -1)
							filterParams["cql_filter"] += selectedOption.value + " = '" + key + "'";
						else{
							filterParams["cql_filter"] += selectedOption.value + " = " + key;
						}
					}
				} else {
					return false;
				}
			}

			var layerName = new String(MapManager.clone.params['LAYERS']);

			layerName = layerName.replace("graph:", "");

			var cqlFilter = filterParams["cql_filter"];

			this.filter = {
				x:MapManager.clone.options.SCHEMA,
				y:layerName,
				query : cqlFilter,
				nome : MapManager.clone.name
			};

			ActionManager.openContentWindow('<iframe style="width:100%;height:392px;padding:0;" frameborder="0" id="feature" src="'+"/ARGO/classes/receiver/GenericReceiver.php?action=columnScreen&z="+this.filter.x+"&y="+this.filter.y+"&x="+this.filter.query+ '"></iframe>',MapManager.map.getLayer(this.layerId).name,600,420);

		}
	},
	
	openColumnWindow : function() {

		var content = document.getElementById('records').innerHTML;

		var number = document.getElementById('number').innerHTML;
		
		var win = new Ext.Window({
			title : this.filter['nome']+": Econtrados "+number+" registros.",
			width : 500,
			height:400,
			autoScroll : false,
			items : [ {
				html : content
			} ]
		});
	
		win.show();

	},

	setCenter:function(coord){
		
		 if(coord != ''){
			var latLong = coord.split(' ');
			
			MapManager.vector.removeAllFeatures();
		
			if(MapManager.projection == 'EPSG:900913')
				MapManager.map.setCenter(new OpenLayers.LonLat(latLong[0], latLong[1]), 18);
			else
				MapManager.map.setCenter(new OpenLayers.LonLat(latLong[0], latLong[1]), 5);
		
			
			MapManager.vector.addFeatures(new OpenLayers.Feature.Vector(new OpenLayers.Geometry.Point(latLong[0], latLong[1])));
		}
		 
	},
	
	disableFilter : function() {

		var filterParams = {
			cql_filter : null
		};

		//filterParams["cql_filter"] = '';

		document.getElementById('filterValueField0').value = '';

		MapManager.clone.setVisibility(false);

		//MapManager.clone.mergeNewParams(params);
		this.mergeNewParams(filterParams);
		
		MapManager.map.removeLayer(MapManager.clone);

		MapManager.clone = "";
		
		MapManager.vector.removeAllFeatures();
		
	},

	clearFilter : function() {
		if (MapManager.clone != null) {

			var filterParams = {
				cql_filter : null
			};

		//	filterParams["cql_filter"] = '';

			MapManager.clone.setVisibility(false);

			for (i = 0; i != this.attCount + 1; i++)
				document.getElementById('filterValueField' + i).value = '';

			MapManager.vector.removeAllFeatures();
			
			this.mergeNewParams(filterParams);
			
		}

	},

	mergeNewParams : function(params) {
		if(params["cql_filter"] == '' || params["cql_filter"] == null){
			MapManager.map.getLayer(this.layerId).mergeNewParams(params);
			MapManager.clone.mergeNewParams(params);
		}		
		else if(!document.getElementById('applyFilterBox').checked){
			MapManager.map.getLayer(this.layerId).mergeNewParams(params);
		}
		else{
			MapManager.clone.mergeNewParams(params);
		}
	},
	
	encode: function(s){
		for(var c, i = -1, l = (s = s.split("")).length, o = String.fromCharCode; ++i < l;
			s[i] = (c = s[i].charCodeAt(0)) >= 127 ? o(0xc0 | (c >>> 6)) + o(0x80 | (c & 0x3f)) : s[i]
		);
		return s.join("");
	},
		
	doRedraw:false,
	
	redrawLayers : function() {

			if(MapAction.doRedraw){
				MapManager.map.layers[7 - MapManager.googleL].redraw(true);MapManager.map.layers[8- MapManager.googleL].redraw(true);MapManager.map.layers[9- MapManager.googleL].redraw(true);MapManager.map.layers[10- MapManager.googleL].redraw(true);
			}
			
	},		
		
	buildPath : function(id) {

					ActionManager.showSpinner();
					MapManager.map.layers[7- MapManager.googleL].setVisibility(true);MapManager.map.layers[8- MapManager.googleL].setVisibility(true);
					var xhr = new XMLHttpRequest();
					xhr.timeout = 9000000;
					xhr.open("GET", "http://localhost:8080/ROUT/receiver?idH="+id+"&request=1", true);
					MapAction.doRedraw = true;
					xhr.onreadystatechange = function() {
						if (xhr.readyState == 4) {
							if (xhr.status == 200) {
								
								MapAction.doRedraw = false;
							    ActionManager.hiddeSpinner();
							    MapAction.getPathValues();

							} else {
								window.alert(xhr.responseText);
								ActionManager.hiddeSpinner();
								MapAction.doRedraw = false;
							}
						}
					};
					xhr.send(null);
					
					
		},	
		
	getPathValues : function() {

			//ActionManager.showSpinner();
			//MapManager.map.layers[7- MapManager.googleL].setVisibility(true);MapManager.map.layers[8- MapManager.googleL].setVisibility(true);
			var xhr = new XMLHttpRequest();
			xhr.timeout = 30;
			xhr.open("GET", "http://localhost:8080/ROUT/receiver?idH="+1+"&request=2", true);
			//MapAction.doRedraw = true;
			xhr.onreadystatechange = function() {
				if (xhr.readyState == 4) {
					if (xhr.status == 200) {
						
						//MapAction.doRedraw = false;
					   // ActionManager.hiddeSpinner();
					    ActionManager.openContentWindow(xhr.responseText,"Resumo da Rota",350,350)
					    var grid = new Ext.ux.grid.TableGrid("the-table", {stripeRows: true});
				        grid.render();

					} else {
					//	window.alert(xhr.responseText);
						//ActionManager.hiddeSpinner();
						//MapAction.doRedraw = false;
					}
				}
			};
			xhr.send(null);
			
			
},
		
	retrieveSequence : function(id) {
			
			var post = {
					sequenceList : id
				};

					//MapAction.doRedraw = true;

			Ext.Ajax.request({
	        url: '/ARGO/classes/receiver/GenericReceiver.php',
			success: function(response)
			{
						//   ActionManager.openContentWindow(response.responseText,'Results',200,70);
						  // MapAction.doRedraw = false;

				ActionManager.openContentWindow(response.responseText,'Setor '+id+': Sequência de Visita',500,400);
				
				//document.getElementById('clusterResult').innerHTML = response.responseText;
				
			},
			failure: function(response){
				   MapAction.doRedraw = false;
			},
			headers: post,
			params: post
			});
			
			
		},
		
	filterCluster : function(id) {
		
		var filterParams = {
				cql_filter : "kume like '%."+id+".%' or subkume ="+id
		};
				
		MapManager.map.layers[10- MapManager.googleL].mergeNewParams(filterParams);
				
	},
	
	filterCQLCluster : function(cql) {
		
		var filterParams = {
				cql_filter : cql
		};
				
		MapManager.map.layers[10- MapManager.googleL].mergeNewParams(filterParams);
				
	},
	
	initCluster : function(id) {
		
		if(clusterNumber>0)
			MapAction.filterCQLCluster("kume like '%.1.%' or subkume = 1");
		else
			MapAction.filterCQLCluster(null);
		
		var post = {
				initCluster : ''
			};

				//MapAction.doRedraw = true;

		Ext.Ajax.request({
        url: '/ARGO/classes/receiver/GenericReceiver.php',
		success: function(response)
		{
					//   ActionManager.openContentWindow(response.responseText,'Results',200,70);
					  // MapAction.doRedraw = false;

		document.getElementById('clusterResult').innerHTML = response.responseText; 
		},
		failure: function(response){
			   MapAction.doRedraw = false;
		},
		headers: post,
		params: post
		});
		
		
		
	},
	
	updateViewNumber : function() {
		
		MapAction.filterCQLCluster(null);
		
	//	if(MapManager['bairro']){
		
		//MapManager['bairro'] = MapManager['bairro']?MapManager['bairro']:'';
		
		if(document.getElementById('bairro').value == MapManager.bairroEmptyText)
			MapManager['bairro'] = '';
			
			var post = {
					bairro:MapManager['bairro'],
					view : document.getElementById('limitFilter').value
				};

							
					Ext.Ajax.request({
					   url: '/ARGO/classes/receiver/GenericReceiver.php',
					   success: function(response)
				        {
						//   ActionManager.openContentWindow(response.responseText,'Results',200,70);
						   MapManager.map.layers[10- MapManager.googleL].redraw(true);
						   
						   document.getElementById('clusterResult').innerHTML = '';

				        },
					   failure: null,
					   headers: post,
					   params: post
					});
					
		//}
	},
	
	restoreCluster : function(id) {
		
		MapAction.filterCQLCluster(null);
			
		var post = {
				clusterList : ''
			};

			//MapAction.doRedraw = true;

		Ext.Ajax.request({
        url: '/ARGO/classes/receiver/GenericReceiver.php',
		success: function(response)
		{
					//   ActionManager.openContentWindow(response.responseText,'Results',200,70);
					  // MapAction.doRedraw = false;

		document.getElementById('clusterResult').innerHTML = response.responseText; 
		},
		failure: function(response){
			   MapAction.doRedraw = false;
		},
		headers: post,
		params: post
		});
		
		
	},
				
	kumeByDistance : function() {

		MapAction.filterCQLCluster(null);
		
		var post = {
				distance : document.getElementById('distanceFilter').value
			};

							MapAction.doRedraw = true;

				Ext.Ajax.request({
				   url: '/ARGO/classes/receiver/GenericReceiver.php',
				   success: function(response)
			        {
					//   ActionManager.openContentWindow(response.responseText,'Results',200,70);
					   MapAction.doRedraw = false;

					   document.getElementById('clusterResult').innerHTML = response.responseText; 
			        },
				   failure: function(response){
					   MapAction.doRedraw = false;
				   },
				   headers: post,
				   params: post
				});
				
				
	},
	
	kumeByNumber : function() {

		MapAction.filterCQLCluster(null);
		
		var post = {
				number: document.getElementById('numberFilter').value
			};

			 					MapAction.doRedraw = true;

				Ext.Ajax.request({
				   url: '/ARGO/classes/receiver/GenericReceiver.php',
				   success: function(response)
			        {
					  // ActionManager.openContentWindow(response.responseText,'Results',600,600);
					   MapAction.doRedraw = false;

					   document.getElementById('clusterResult').innerHTML = response.responseText;
			        },
				   failure: function(response){
					   MapAction.doRedraw = false;
				   },
				   headers: post,
				   params: post
				});
				
				
	},
	
	kumeByTime : function() {

		MapAction.filterCQLCluster(null);
		
		var post = {
				time : document.getElementById('timeFilter').value,
				x : document.getElementById('xA').value,
				y : document.getElementById('xV').value
			};

					MapAction.doRedraw = true;

				Ext.Ajax.request({
				   url: '/ARGO/classes/receiver/GenericReceiver.php',
				   success: function(response)
			        {
					//   ActionManager.openContentWindow(response.responseText,'Results',200,70);
					   MapAction.doRedraw = false;

					   document.getElementById('clusterResult').innerHTML = response.responseText;
			        },
				   failure: function(response){
					   MapAction.doRedraw = false;
				   },
				   headers: post,
				   params: post
				});
				
				
	},
	
	restoreSubCluster : function(id) {
		
		var filterParams = {
				cql_filter : "kume like '%."+id+".%' OR subkume = "+id
		};
				
		MapManager.map.layers[10- MapManager.googleL].mergeNewParams(filterParams);
			
		var post = {
				subClusterList : id
			};

			//MapAction.doRedraw = true;

		Ext.Ajax.request({
        url: '/ARGO/classes/receiver/GenericReceiver.php',
		success: function(response)
		{
					//   ActionManager.openContentWindow(response.responseText,'Results',200,70);
					  // MapAction.doRedraw = false;

		document.getElementById('subClusterResult').innerHTML = response.responseText; 
		},
		failure: function(response){
			   MapAction.doRedraw = false;
		},
		headers: post,
		params: post
		});
		
		
	},
				
	subKumeByDistance : function(id) {

		MapAction.filterCQLCluster("subkume = "+id);
		
		var post = {
				subKume : id,
				distance : document.getElementById('subDistanceFilter').value,
				z: document.getElementById('doi').value
			};

							MapAction.doRedraw = true;

				Ext.Ajax.request({
				   url: '/ARGO/classes/receiver/GenericReceiver.php',
				   success: function(response)
			        {
					//   ActionManager.openContentWindow(response.responseText,'Results',200,70);
					   MapAction.doRedraw = false;

					   document.getElementById('subClusterResult').innerHTML = response.responseText; 
			        },
				   failure: function(response){
					   MapAction.doRedraw = false;
				   },
				   headers: post,
				   params: post
				});
				
				
	},
	
	subKumeByNumber : function(id) {

		MapAction.filterCQLCluster("subkume = "+id);
		
		var post = {
				subKume : id,
				number : document.getElementById('subNumberFilter').value,
				z: document.getElementById('doi').value
			};

						MapAction.doRedraw = true;

				Ext.Ajax.request({
				   url: '/ARGO/classes/receiver/GenericReceiver.php',
				   success: function(response)
			        {
					  // ActionManager.openContentWindow(response.responseText,'Results',600,600);
					   MapAction.doRedraw = false;

					   document.getElementById('subClusterResult').innerHTML = response.responseText;
			        },
				   failure: function(response){
					   MapAction.doRedraw = false;
				   },
				   headers: post,
				   params: post
				});
				
				
	},
	
	subKumeByTime : function(id) {

		MapAction.filterCQLCluster("subkume = "+id);
		
		var post = {
				subKume : id,
				time : document.getElementById('subTimeFilter').value,
				x : document.getElementById('subXA').value,
				y : document.getElementById('subXV').value,
				z : document.getElementById('doi').value
			};

							MapAction.doRedraw = true;

				Ext.Ajax.request({
				   url: '/ARGO/classes/receiver/GenericReceiver.php',
				   success: function(response)
			        {
					//   ActionManager.openContentWindow(response.responseText,'Results',200,70);
					   MapAction.doRedraw = false;

					   document.getElementById('subClusterResult').innerHTML = response.responseText;
			        },
				   failure: function(response){
					   MapAction.doRedraw = false;
				   },
				   headers: post,
				   params: post
				});
				
				
	},
	
	openSubClusterScreen : function(kume) {
		
		var clusterForm = MapManager.returnSubClusterForm(kume);
		
		var subClusterResult = {autoScroll: true,region: 'center',border: true, id:'subClusterResult'};

		var win = new Ext.Window({
			title : "Setor "+kume,
			width : 500,
			height:600,
			autoScroll : false,
			items : [ clusterForm,subClusterResult ]
		});
	
		win.show();

		MapAction.restoreSubCluster(kume);
		
	},
	
	openLegendWindow : function(kume) {

		var html = "<div style='margin-top:2px;float:left;width:95%;'><div style='width:10%;float:left;background-color:#E8E8E8;'><img  style='float:left;' src='/ARGO/images/byDistance.gif'/></div><div style='background-color:#E8E8E8;width:90%;float:left;padding:3.5px 0 3.1px 0;'> - Roteirizar um setor.</div></div>";
		html += "<div style='margin-top:2px;float:left;width:95%;'><div style='width:10%;float:left;background-color:#E8E8E8;'><img  style='float:left;' src='/ARGO/images/number.gif'/></div><div style='background-color:#E8E8E8;width:90%;float:left;padding:3.5px 0 3.1px 0;'> - Retornar número de clientes.</div></div>";

		html += "<div style='margin-top:2px;float:left;width:95%;'><div style='width:10%;float:left;background-color:#E8E8E8;'><img  style='float:left;' src='/ARGO/images/details.gif'/></div><div style='background-color:#E8E8E8;width:90%;float:left;padding:3.5px 0 3.1px 0;'> - Dividir um setor em dias.</div></div>";

		html += "<div style='margin-top:2px;float:left;width:95%;'><div style='width:10%;float:left;background-color:#E8E8E8;'><img  style='float:left;' src='/ARGO/images/cliente.gif'/></div><div style='background-color:#E8E8E8;width:90%;float:left;padding:3.5px 0 3.1px 0;'> - Retornar a sequência de visita em um dia.</div></div>";

	
		var win = new Ext.Window({
			title : "Legenda dos Botões",
			width : 300,
			height:130,
			autoScroll : false,
			items : [ {autoScroll: true,region: 'center',border: false, id:'legendWindowContent',html:'<div style="background-color:#E8E8E8; width:299px">'+html+'</div>'} ]
		});
	
		win.show();

	},
	
	sendClusterNumberRequest : function(title,gid) {

		var post = {
			clusterNumber : gid
		};

				
			Ext.Ajax.request({
			   url: '/ARGO/classes/receiver/GenericReceiver.php',
			   success: function(response)
		        {
				   ActionManager.openContentWindow(response.responseText,title,200,70);
		        },
			   failure: null,
			   headers: post,
			   params: post
			});


	},
	
	sendSubClusterNumberRequest : function(title,gid) {

		var post = {
			subClusterNumber : gid
		};

		
			
			Ext.Ajax.request({
			   url: '/ARGO/classes/receiver/GenericReceiver.php',
			   success: function(response)
		        {
				   ActionManager.openContentWindow(response.responseText,title,200,70);
		        },
			   failure: null,
			   headers: post,
			   params: post
			});


	},
	
	/////EDGE MERGER
	
	openEdgeMergerScreen : function() {
		
		var form = new Ext.FormPanel({
	        labelWidth: 75, // label settings here cascade unless overridden
		       // url:'save-form.php',
		        frame:true,
		        bodyStyle:'padding:5px 5px 0',
		       // width: 350,
		        region:'north',
		        items: [{
		            xtype:'fieldset',
		            title: 'Merger',
		            collapsible: true,
		            collapsed: false,
		            autoHeight:true,
		            defaultType: 'textfield',
		            items :[
		               {
		                    fieldLabel: 'ONE_WAY',
		                //    name: 'home',
		                    width: 130,
		                    id:'ONE_WAY',
		                    value: ''
		                },
		                {
		                    fieldLabel: 'NODES',
		                //    name: 'home',
		                    width: 130,
		                    id:'NODES',
		                    value: ''
		                },
		                {
		                    fieldLabel: 'GEOM_1',
		                //    name: 'home',
		                    width: 130,
		                    id:'GEOM_1',
		                    value: ''
		                },
		                {
		                    fieldLabel: 'GEOM_2',
		                //    name: 'home',
		                    width: 130,
		                    id:'GEOM_2',
		                    value: ''
		                }
		                
		                
		            ],
		        
		        
		            
		        },
		        
		        ]		        
		
		    });
	
		var win = new Ext.Window({
			title : "Edge Merger",
			width : 300,
			y:window.innerHeight/10,
			x:window.innerWidth/10,
			autoScroll : false,
			items : [ form ],
			buttons: [
			          
			          'DB',
					  {iconCls : 'db',handler : function(){
						  document.getElementById('ONE_WAY').value = 'DB';
					  }},
			          'TF',
					  {iconCls : 'tf',handler : function(){
						  document.getElementById('ONE_WAY').value = 'TF';
					  }},
					  'FT',
					  {iconCls : 'ft',handler : function(){
						  document.getElementById('ONE_WAY').value = 'FT';
										  }},
			          
			          {iconCls : 'saveButton',handler : function(){MapAction.sendEdgeMerger();}},
			          {iconCls : 'gridRemove',handler : function(){
			        	  
			        	  document.getElementById('GEOM_2').value = '';
			        	  document.getElementById('GEOM_1').value = '';
			        	  document.getElementById('ONE_WAY').value = '';
			        	  document.getElementById('NODES').value = '';
			          }}
			          
			]
		});
	
		win.show();
		
	},
	
	sendEdgeMerger: function() {
				
		 var post = {
				 edgeMerge:'',
				 GEOM_1:document.getElementById('GEOM_1').value,
				 GEOM_2:document.getElementById('GEOM_2').value,
				 ONE_WAY:document.getElementById('ONE_WAY').value,
				 NODES:document.getElementById('NODES').value
		};

	
		Ext.Ajax.request({
		   url: '/ARGO/classes/receiver/GenericReceiver.php',
		   success: function(response)
		   {	
			  // MessageManager.successMessage('Ok!');
			   
			   MapManager.map.getLayersByName("Quinas")[0].redraw(true);
			   
			   MapManager.map.getLayersByName("Trechos")[0].redraw(true);
			   
		   },
		   failure: function(response){
				   //MapAction.doRedraw = false;
		   },
		   headers: post,
		   params: post
		});
	
	 
		
		
	},
	
	doNodeFinder : function(evt) {

		if (evt) {

			var lonLat = MapManager.map.getLonLatFromPixel(evt.xy);
			
			var post = {
					nearestNode : '',
					x:lonLat.lat,
					y:lonLat.lon
				};
			
         
					
			Ext.Ajax.request({
				   url: '/ARGO/classes/receiver/GenericReceiver.php',
				   success: function(response)
			        {
					   
					   if(document.getElementById('GEOM_1').value != '' && document.getElementById('GEOM_2').value != ''){
						   
						   document.getElementById('GEOM_1').value = '';
						   document.getElementById('GEOM_2').value = '';
						   
					   }
					   
					   if(document.getElementById('GEOM_1').value != ''){
						   
						   document.getElementById('GEOM_2').value = response.responseText;
						   
						   var ONE_WAY = document.getElementById('ONE_WAY').value;
						   
						   if(ONE_WAY == 'DB' || ONE_WAY == ''){
							   
							   document.getElementById('NODES').value += '='+response.responseText;
							   
						   }else if(ONE_WAY == 'TF' || ONE_WAY == 'FT'){
							   
							   document.getElementById('NODES').value += '-'+response.responseText;
						   }
						   
					   }
					   else{
						   
						   document.getElementById('GEOM_1').value = response.responseText;
						   document.getElementById('NODES').value = response.responseText;
						   
					   }
					   
					   
			        },
				   falure: function(response){window.alert('HII')},
				   headers: post,
				   params: post
			});
			
		} 
	},

	toggleNodeFinderMode : function() {
		if (MapManager.nodeFinder.active) {
			MapManager.nodeFinderEventHandler.activate();
		} else {
			MapManager.nodeFinderEventHandler.deactivate();
		}
	},
	
	/////ROUT MERGER

	openRoutMergerScreen : function() {
		
		var form = new Ext.FormPanel({
	        labelWidth: 75, // label settings here cascade unless overridden
		       // url:'save-form.php',
		        frame:true,
		        bodyStyle:'padding:5px 5px 0',
		       // width: 350,
		        region:'north',
		        items: [{
		            xtype:'fieldset',
		            title: 'TRECHO',
		            collapsible: true,
		            collapsed: false,
		            autoHeight:true,
		            defaultType: 'textfield',
		            items :[
		               {
		                    fieldLabel: 'CÓDIGO',
		                //    name: 'home',
		                    width: 130,
		                    id:'rout_gid',
		                    value: ''
		                },
		                {
		                    fieldLabel: 'NOME',
		                //    name: 'home',
		                    width: 130,
		                    id:'rout_name',
		                    value: ''
		                },
		                
		                
		                
		            ],
		        
		        
		            
		        },
		        
		        ]		        
		
		    });
	
		var win = new Ext.Window({
			title : "Edge Merger",
			width : 300,
			y:window.innerHeight/10,
			x:window.innerWidth/10,
			autoScroll : false,
			items : [ form ],
			buttons: [
			          
			          {iconCls : 'saveButton',handler : function(){MapAction.sendRoutMerger();}},
			          {iconCls : 'gridRemove',handler : function(){
			        	  MapAction.sendRoutDrop();
			        	  document.getElementById('rout_gid').value = '';
			        	  document.getElementById('rout_name').value = '';
			          }}
			          
			]
		});
	
		win.show();
		
	},
	
	sendRoutMerger : function() {

		 var post = {
				 routMerge:'',
				 routGid:document.getElementById('rout_gid').value
		};

	
		Ext.Ajax.request({
		   url: '/ARGO/classes/receiver/GenericReceiver.php',
		   success: function(response)
		   {	
			   MessageManager.successMessage(response.responseText);
			   
			  // MapManager.map.getLayersByName("Quinas")[0].redraw(true);
			   
			  // MapManager.map.getLayersByName("Trechos")[0].redraw(true);
			   
			   MapManager.map.getLayersByName("Rota")[0].redraw(true);
			   
		   },
		   failure: function(response){
				   //MapAction.doRedraw = false;
		   },
		   headers: post,
		   params: post
		});
		
	},
	
	sendRoutDrop : function() {

		 var post = {
				 routDrop:'',
				 routGid:document.getElementById('rout_gid').value
		};

	
		Ext.Ajax.request({
		   url: '/ARGO/classes/receiver/GenericReceiver.php',
		   success: function(response)
		   {	
			   MessageManager.successMessage(response.responseText);
			   
			  // MapManager.map.getLayersByName("Quinas")[0].redraw(true);
			   
			  // MapManager.map.getLayersByName("Trechos")[0].redraw(true);
			   
			   MapManager.map.getLayersByName("Rota")[0].redraw(true);
			   
		   },
		   failure: function(response){
				   //MapAction.doRedraw = false;
		   },
		   headers: post,
		   params: post
		});
		
	},
	
	doRoutMerger : function(evt) {

		if (evt) {

			var lonLat = MapManager.map.getLonLatFromPixel(evt.xy);
			
			var post = {
					routFinder : '',
					x:lonLat.lat,
					y:lonLat.lon
				};
			
         
					
			Ext.Ajax.request({
				   url: '/ARGO/classes/receiver/GenericReceiver.php',
				   success: function(response)
			        {
					   
					   var data = response.responseText.split('&&');

					   document.getElementById('rout_gid').value = data[0];
					   document.getElementById('rout_name').value = data[1];
					   
			        },
				   falure: function(response){window.alert('HII')},
				   headers: post,
				   params: post
			});
			
		} 
	},
	
	toggleRoutMergerMode : function() {
		if (MapManager.routMerger.active) {
			MapManager.routMergerEventHandler.activate();
		} else {
			MapManager.routMergerEventHandler.deactivate();
		}
	},
	
	/////GeoCod
	
	geocode:function(id){
		
		var address = "Fortaleza "+document.getElementById(id).value;
		
		geocoder.geocode(
				
				{'address' : address}, 
		        
				function(results, status) {
					var x = status;
					if (status == google.maps.GeocoderStatus.OK) {
			
						var lonLat = null;
						
						var lat = results[0].geometry.location.lat().toFixed(6);
						
						var long= results[0].geometry.location.lng().toFixed(6);
						
						if(MapManager.projection == 'EPSG:900913'){
							
							lonLat = new OpenLayers.LonLat(long, lat).transform(new OpenLayers.Projection("EPSG:4326"),new OpenLayers.Projection("EPSG:900913"));
						}
						
						if(lonLat){
							MapAction.setCenter(lonLat.lon+" "+ lonLat.lat);
						}
						else
							MapAction.setCenter(long+" "+ lat);
						
					} else {
						alert("Invalid address!");
					}
				
				});
		
	},
	
	geocodeTableString:function(value){
		
		var address = "Fortaleza "+value;
		
		geocoder.geocode(
				
				{'address' : address}, 
		        
				function(results, status) {
					var x = status;
					if (status == google.maps.GeocoderStatus.OK) {
			
						var lonLat = null;
						
						var lat = results[0].geometry.location.lat().toFixed(6);
						
						var long= results[0].geometry.location.lng().toFixed(6);
						
						if(MapManager.projection == 'EPSG:900913'){
							
							lonLat = new OpenLayers.LonLat(long, lat).transform(new OpenLayers.Projection("EPSG:4326"),new OpenLayers.Projection("EPSG:900913"));
						}
						
						if(lonLat){
							MapAction.setCenter(lonLat.lon+" "+ lonLat.lat);
						}
						else
							MapAction.setCenter(long+" "+ lat);
						
					} else {
						alert("Invalid address!");
					}
				
				});
		
	},
	
	drawGeocodBox : function(bbox) {

		//MapControls.box.deactive();
		
		var bounds = bbox.getBounds();

		var feature = new OpenLayers.Feature.Vector(bounds.toGeometry());
		
		MapManager.vector.addFeatures(feature);
		
        var wkt = new OpenLayers.Format.WKT();
        
        var str = "SRID=900913;"+wkt.write(feature);
				
		var post = {
			geocodbox: str
		};



		Ext.Ajax.request({
		   url: '/ARGO/classes/receiver/GenericReceiver.php',
		   success: function(response)
		   {	
			   
			   var win = new Ext.Window({
					title : "Pontos de Atendimento",
					width : 480,
					height:370,
					autoScroll : false,
			        x:window.innerWidth/10,
					html:'<div id="geocodbox"></div>'
				});
			
			   win.show();
			   
			   document.getElementById('geocodbox').innerHTML = response.responseText;
			   
		   },
		   failure: function(response){
			   //MapAction.doRedraw = false;
		   },
		   headers: post,
		   params: post
		});
		
	}, 
	
	reverseGeoCod:function(coord){
		
		 if(coord != ''){
			
			 var latLong = coord.split(' ');
			
			 var post = {
					reverseGeocod:'',
					lat: latLong[1],
					long:latLong[0],
			};

		
			Ext.Ajax.request({
			   url: '/ARGO/classes/receiver/GenericReceiver.php',
			   success: function(response)
			   {	
						   
					   var win = new Ext.Window({
							title : "Geocodificação Reversa",
							width : 200,
					        x:window.innerWidth/10,
							height:150,
							autoScroll : false,
							html:response.responseText
						});
						
					   win.show();
						   
			   },
			   failure: function(response){
					   //MapAction.doRedraw = false;
			   },
			   headers: post,
			   params: post
			});
		
		 }
		 
	},

};