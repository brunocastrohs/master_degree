ActionManager = {

	isInsert : false,
	north:null,
	form : null,
	fid : null,
	featureView : null,
	k : null,
	realm: 'ARGO',

	//geocod
	mergeGeocod : function(lat,long,gid) {

		
		var post = {
				lat : lat,
				long : long,
				gid : gid,
			action : 'geocod',
		};

		page = '/ARGO/classes/receiver/GenericReceiver.php';

		PLX.Request({
			url : page,
			method : "post",
			target : 'here',
			params : post
		});

		
	},
	

	 geocoden:function(address) {

		geocoder.geocode(
				
				{'address' : address['a']}, 
		        
				function(results, status) {
					var x = status;
					if (status == google.maps.GeocoderStatus.OK) {
				//		document.getElementById('geo').innerHTML+= '<div>'+address+'='+results[0].geometry.location.lat().toFixed(6)+' '+results[0].geometry.location.lng().toFixed(6)+'</div><br/>';
						ActionManager.mergeGeocod(results[0].geometry.location.lat().toFixed(6),results[0].geometry.location.lng().toFixed(6),address['gid']);

					} else {
						document.getElementById('geo').innerHTML+=address['a']+"=Invalid address!<br/>";
					}
				
				});
	},
	

	 readDBAddress: function(){
		
		var i =0;
		
		for(i = 0; i != dbAddress.length; i++)
			ActionManager.geocoden(dbAddress[i]);
		
		document.getElementById('geo').innerHTML = str;
			
		
	},
	
	// geom statics
	sendRequest : function(data, action, target) {

		var geom = '';

		if (GeomManager.hasGeom) {
			geom = data[GeomManager.georg];
			data[GeomManager.georg] = '';
		}

		var post = {
			x : FormManager.x,
			y : FormManager.y,
			z : FormManager.z,
			action : action,
			values : data,
			geom : geom,
			georg : GeomManager.georg,
			epsg : GeomManager.epsg
		};

		page = '/ARGO/classes/receiver/GenericReceiver.php';

		PLX.Request({
			url : page,
			method : "post",
			target : target,
			preloader : 'pr',
			params : post
		});

		if (GeomManager.hasGeom)
			data[GeomManager.georg] = geom;

	},
	
	sendReportRequest : function(type) {
		
		var x = document.getElementById('boxes')
		
		var i = 0;
		
		var data = "";
		//x.children->children.firstChild.checked;
		
		var isFirst = true;
		
		for(i = 0; i != x.children.length;i++){
			if(x.children[i].firstChild.checked){
				
				if(isFirst){
					data+=x.children[i].firstChild.id;
				}
				else{
					data+=","+x.children[i].firstChild.id;
				}
				isFirst = false;
			}
				
		}
		
		window.open('/ARGO/classes/receiver/GenericReceiver.php?action=reportBean&type='+type+'&z='+FormManager.z+'&y='+FormManager.y+'&k='+data+'&x='+FormManager.x);
		
	},
	
	sendNumberRequest : function(data) {

		var post = {
			x : data['x'],
			y : data['y'],
			action : 'number'
		};

		 Ext.Ajax.timeout = 9000000;  
			
			Ext.Ajax.on('beforerequest', ActionManager.showSpinner, this);
			Ext.Ajax.on('requestcomplete', ActionManager.hiddeSpinner, this);
			Ext.Ajax.on('requestexception', ActionManager.hiddeSpinner, this);
			
			Ext.Ajax.request({
			   url: '/ARGO/classes/receiver/GenericReceiver.php',
			   success: function(response)
		        {
				   ActionManager.openContentWindow(response.responseText,data['title'],200,70);
		        },
			   failure: null,
			   headers: post,
			   params: post
			});


	},

	sendPositionRequest : function() {

		var post = {
			action : 'checkPosition',
		};

		page = '/ARGO/classes/receiver/GenericReceiver.php';

		PLX.Request({
			url : page,
			method : "post",
			params : post
		});

	},

	sendGISRequest : function(data, action, target) {

		var post = {
			x : data.x,
			y : data.y,
			action : action,
			values : data.query
		};

		page = '/ARGO/classes/receiver/GenericReceiver.php';

		PLX.Request({
			url : page,
			method : "post",
			target : target,
			preloader : 'pr',
			params : post
		});

	},

	sendGISFormRequest : function(feature) {

		var feature = feature.split('.');

		window.location = 'http://localhost:80/'+ActionManager.realm+'/classes/receiver/GenericReceiver.php?action=gisUpdateForm&k='+ ActionManager.k + '&y=' + feature[0] + '&gid=' + feature[1];

	},
	// geom statics
	sendForm : function(action, target) {

		var keys = this.form.getForm().items.keys;

		var data = {};

		var geom = '';

		var e = null;

		for (i = 0; i != keys.length; i++) {
			if (keys[i] != 'x0' && keys[i] != 'y0'
					&& keys[i] != GeomManager.georg) {
				e = document.getElementById(keys[i]);
				if (e['type'] == 'checkbox' && keys[i].indexOf('atend') != -1 && e.checked){
					data['kumeDays'] += keys[i];
					data['subkume'] = FormManager['subkume'];
				}
				else if (e['type'] == 'checkbox' && keys[i].indexOf('restricao') != -1 && e.checked){
					data['kumeDaysRestriction'] += keys[i];
				}
				else if (e['type'] == 'checkbox' && !(keys[i].indexOf('restricao') != -1 | keys[i].indexOf('atend') != -1))
					data[keys[i]] = e.checked;
				else if(FormManager.fs.indexOf(keys[i]) != -1){
					for(j=0;j!= FormManager.fkData[keys[i]].length;j++)
						if(FormManager.fkData[keys[i]][j][1] == e.value){
							data[keys[i]] = FormManager.fkData[keys[i]][j][0];
						}
					
					
				}
				else if(e['type'] != 'checkbox')
					data[keys[i]] = e.value;
			}
		}

		if (GeomManager.hasGeom && GeomManager.type == 'POINT')
			geom = GeomManager.generateGeoJson();
		// else if (GeomManager.hasGeom)
		// geom = document.getElementById('geom').value;

		var post = {
			x : FormManager.x,
			y : FormManager.y,
			z : FormManager.z,
			o : FormManager.o,
			action : action,
			values : data,
			geom : geom,
			georg : GeomManager.georg,
			epsg : GeomManager.epsg
		};

		page = '/ARGO/classes/receiver/GenericReceiver.php';

		PLX.Request({
			url : page,
			method : "post",
			target : target,
			preloader : 'pr',
			params : post
		});

	},

	checkFID : function(id) {

		var view = null;

		var x = null;

		var url = new String(window.location);

		url = url.split('&');

		var view = url[url.length - 1];

		var k = url[url.length - 2];
		
		var r = url[url.length - 3];

		this.featureView = view.replace('view=', "");

		this.k = k.replace('k=', "");
		
		this.realm = r.replace('realm=', "");

	},

	sendByFID : function(value) {
		if (this.featureView == 'false')
			ActionManager.sendGISFormRequest(value)
	},

	sendByViewFID : function(name, value, typeName) {
		if (typeName == this.featureView)
			ActionManager.sendGISFormRequest(name + "." + value);
	},

	openWindow : function(url) {

		var content = '<iframe style="width:100%;height:392px;padding:0;" frameborder="0" id="feature" src="'+ url + '"></iframe>';

		var win = new Ext.Window({
			// title : this.filter['nome'],
			width : 600,
			height : 420,
			items : [ {
				html : content
			} ]
		});

		win.show();

	},
	
	openContentWindow : function(content,title,width,height) {
		
		var win = new Ext.Window({
			title : title,
			width : width,
			height : height,
			items : [ {
				html : content
			} ]
		});

		win.show();

	},

	sendCrossRequest : function() {

		// var v =
		// [{pid:'653',r:'1010'},{pid:'8322',r:'1011'},{pid:'8561',r:'1012'},{pid:'7229',r:'1013'},{pid:'7339',r:'1014'},{pid:'7909',r:'1015'}];

		// var i = 0;
		var xhr = new XMLHttpRequest();
		xhr.open("GET", "http://72.55.153.186/findnet/modulos/ws.php?eqp_srn="+ ActionManager.v[ActionManager.index]['pid'], true);
		xhr.onreadystatechange = function() {
			if (xhr.readyState == 4) {
				if (xhr.status == 200) {
					var r = xhr.responseText;
					var params = '';
					params = r.split(',');
					if (params != '' && params[2] != '' && params[3] != '')
						ActionManager.mergeCrossRequest(params[2], params[3],ActionManager.v[ActionManager.index]['r'])
				} else {
					// str += "ERROR";
				}
			}
		};
		xhr.send(null);

	},

	mergeCrossRequest : function(lat, lon, r) {

		var post = {
			action : 'crossPosition',
			lat : lat,
			lon : lon,
			r : r
		};

		page = '/ARGO/classes/receiver/GenericReceiver.php';

		PLX.Request({
			url : page,
			method : "post",
			params : post
		});
		
		if(ActionManager.index < ActionManager.v.length-1){
			ActionManager.index++;
			ActionManager.sendCrossRequest(ActionManager.index);
		}
		else{
			ActionManager.index=0;
		}
		
	},
	
	showSpinner:function(){
		document.getElementById('pr').style.visibility='visible';
	},
	
	hiddeSpinner:function(){
		document.getElementById('pr').style.visibility='hidden';
	}

};