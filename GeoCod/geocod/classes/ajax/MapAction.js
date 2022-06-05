MapAction = {
	
	filter : {
		y : "",
		params : "",
		nome : ""
	},

	enableFilter : function() {

		if(MapManager.clone == null){
		
		MapManager.clone = MapManager.map.layers[1].clone();
		
		MapManager.clone.mergeNewParams({styles : 'line'});
	
		MapManager.map.addLayer(MapManager.clone);
		}	
		
		if (MapManager.clone != null) {

			MapManager.vector.removeAllFeatures();
			
			var filter = document.getElementById('filter').value;
			
			var filterParams = {
				cql_filter : "name like '%"+filter+"%'"
			};

			MapAction.mergeNewParams(filterParams);

			MapManager.clone.setVisibility(true);

			ActionManager.sendGISRequest(filter);

		}
	},
	
	enablePhoneFilter : function() {

		if(MapManager.clone == null){
		
		MapManager.clone = MapManager.map.layers[1].clone();
		
		MapManager.clone.mergeNewParams({styles : 'line'});
	
		MapManager.map.addLayer(MapManager.clone);
		}	
		
		if (MapManager.clone != null) {

			MapManager.vector.removeAllFeatures();
			
			var filter = document.getElementById('filter').value;
			
			var filterParams = {
				cql_filter : "name like '%"+filter+"%'"
			};

			MapAction.mergeNewParams(filterParams);

			MapManager.clone.setVisibility(true);

			ActionManager.sendPhoneGISRequest(filter);

		}
	},
	
	enableLevenshtainFilter : function() {

		if(MapManager.clone == null){
		
		MapManager.clone = MapManager.map.layers[1].clone();
		
		MapManager.clone.mergeNewParams({styles : 'line'});
	
		MapManager.map.addLayer(MapManager.clone);
		}	
		
		if (MapManager.clone != null) {

			MapManager.vector.removeAllFeatures();
			
			var filter = document.getElementById('filter').value;
			
			var filterParams = {
				cql_filter : "name like '%"+filter+"%'"
			};

			MapAction.mergeNewParams(filterParams);

			MapManager.clone.setVisibility(true);

			ActionManager.sendLevenshteinGISRequest(filter);

		}
	},

	retrieveRecords : function() {

		var content = document.getElementById('records').innerHTML;

		var size = parseInt(document.getElementById('size').innerHTML) * 100;
		
		var win = new Ext.Window({
			title : this.filter['nome'],
			width : size,
			height:400,
			autoScroll : true,
			items : [ {
				html : content
			} ]
		});
	
		win.show();

	},

	setCenter:function(coord){
		
		 if(coord != ''){
			var latLong = coord.split(' ');
		
			MapManager.map.setCenter(new OpenLayers.LonLat(latLong[0], latLong[1]), 14);
		
			MapManager.vector.addFeatures(new OpenLayers.Feature.Vector(new OpenLayers.Geometry.Point(latLong[0], latLong[1])));
		}
		 
	},

	clearFilter : function() {
		if (MapManager.clone != null) {

			var filterParams = {
				cql_filter : null
			};

			filterParams["cql_filter"] = '';

			MapManager.clone.setVisibility(false);

			for (i = 0; i != this.attCount + 1; i++)
				document.getElementById('filter' + i).value = '';

			MapManager.vector.removeAllFeatures();
			
			this.mergeNewParams(filterParams);
		}

	},

	mergeNewParams : function(params) {
		MapManager.clone.mergeNewParams(params);
	}

};