ActionManager = {

	isInsert : false,
	form : null,
	fid : null,
	
	mergeGeocod : function(lat,long,gid) {

	
		var post = {
				lat : lat,
				long : long,
				gid : gid,
			action : 'geocod',
		};

		page = '/GeoCod/classes/receiver/GenericReceiver.php';

		PLX.Request({
			url : page,
			method : "post",
			target : 'here',
			params : post
		});

		
	},

	sendLatLongRequest : function(data) {

		var post = {
			param : data,
			latLong: 'latLong'
		};

		page = '/GeoCod/classes/receiver/GenericReceiver.php';

		PLX.Request({
			url : page,
			method : "post",
			preloader : "pr",
			params : post
		});

	},

	sendGISRequest : function(data) {

		var post = {
			param : data
		};

		page = '/GeoCod/classes/receiver/GenericReceiver.php';

		PLX.Request({
			url : page,
			method : "post",
			target : 'records',
			preloader : "pr",
			params : post
		});

	},
	
	sendPhoneGISRequest : function(data) {

		var post = {
			param : data,
			phone: 'phone'
		};

		page = '/GeoCod/classes/receiver/GenericReceiver.php';

		PLX.Request({
			url : page,
			method : "post",
			target : 'phoneRecords',
			preloader : "pr",
			params : post
		});

	},
	
	sendLevenshteinGISRequest : function(data) {

		var post = {
			param : data,
			levenshtein: 'levenshtein'
		};

		page = '/GeoCod/classes/receiver/GenericReceiver.php';

		PLX.Request({
			url : page,
			method : "post",
			target : 'levenshtainRecords',
			preloader : "pr",
			params : post
		});

	}



};