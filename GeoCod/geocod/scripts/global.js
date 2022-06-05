//var map = new google.maps.Map();
var	geocoder = new google.maps.Geocoder();
//var	marker = new google.maps.Marker({map : map});
var str = "";


function initialize() {
	var latlng = new google.maps.LatLng(1.10, 1.10);
	var myOptions = {
		zoom : 5,
		center : latlng,
		mapTypeId : google.maps.MapTypeId.ROADMAP
	};
	map = new google.maps.Map(document.getElementById("latlongmap"), myOptions);
	geocoder = new google.maps.Geocoder();
	marker = new google.maps.Marker({
		position : latlng,
		map : map
	});

	map.streetViewControl = false;

	google.maps.event.addListener(map, 'click', function(event) {
		marker.setPosition(event.latLng);

		var yeri = event.latLng;
		document.getElementById('lat').value = yeri.lat().toFixed(6);
		document.getElementById('lng').value = yeri.lng().toFixed(6);
	});

	google.maps.event.addListener(map, 'mousemove', function(event) {
		var yeri = event.latLng;
		document.getElementById("mlat").value = yeri.lat().toFixed(6);
		document.getElementById("mlong").value = yeri.lng().toFixed(6);
	});

}

function codeAddress() {
	var address = document.getElementById("gadres").value;
	
	geocoder.geocode(
			{'address' : address}, 
	        function(results, status) {
				if (status == google.maps.GeocoderStatus.OK) {
					map.setCenter(results[0].geometry.location);
					document.getElementById('lat').value = results[0].geometry.location.lat().toFixed(6);
					document.getElementById('lng').value = results[0].geometry.location.lng().toFixed(6);
			
					var latlong = "(" + results[0].geometry.location.lat().toFixed(6)+ " , " + +results[0].geometry.location.lng().toFixed(6)+ ")";

					var infowindow = new google.maps.InfoWindow({content : latlong});

					marker.setPosition(results[0].geometry.location);
					
					map.setZoom(16);

					google.maps.event.addListener(marker, 'click', function() {
						infowindow.open(map, marker);
					});

				} else {
					alert("Lat and long cannot be found.");
				}
			});
}


function geocoden(address) {
	

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
}


function readDBAddress(){
	
	var i =0;
	
	for(i = 0; i != dbAddress.length; i++)
		geocoden(dbAddress[i]);
	
	document.getElementById('geo').innerHTML = str;
		
	
}