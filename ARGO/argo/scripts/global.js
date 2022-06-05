var vectors;
var box;
var transform;

function endDrag(bbox) {
	
	var bounds = bbox.getBounds();

	MapManager.vector.addFeatures(new OpenLayers.Feature.Vector(bounds.toGeometry()));
	
	b = bounds.clone();
	
	minlon = toPrecision(map.getZoom(), b.left);
	minlat = toPrecision(map.getZoom(), b.bottom);
	maxlon = toPrecision(map.getZoom(), b.right);
	maxlat = toPrecision(map.getZoom(), b.top);
	
	//Send this

}

function drawBox(bounds) {
	var feature = new OpenLayers.Feature.Vector(bounds.toGeometry());

	vectors.addFeatures(feature);
}

function toPrecision(zoom, value) {
	var decimals = Math.pow(10, Math.floor(zoom / 3));
	return Math.round(value * decimals) / decimals;
}

function setBounds(bounds) {
	if (bounds == null) {
		document.getElementById("bbox_result").innerHTML = "";

	} else {
		b = bounds.clone();
				
		minlon = toPrecision(map.getZoom(), b.left);
		minlat = toPrecision(map.getZoom(), b.bottom);
		maxlon = toPrecision(map.getZoom(), b.right);
		maxlat = toPrecision(map.getZoom(), b.top);

		document.getElementById("bbox_result").innerHTML = "minlon=" + minlon
				+ ", " + "minlat=" + minlat + ", " + "maxlon=" + maxlon + ", "
				+ "maxlat=" + maxlat;
	}
}

function init() {
	map = new OpenLayers.Map("mapdiv");
	var openstreetmap = new OpenLayers.Layer.OSM();
	map.addLayer(openstreetmap);

	var lonlat = new OpenLayers.LonLat(-39.5, -3.4).transform(
			new OpenLayers.Projection("EPSG:4326"), // transform from WGS 1984
			new OpenLayers.Projection("EPSG:900913") // to Spherical Mercator
	);

	var zoom = 11;

	vectors = new OpenLayers.Layer.Vector("Vector Layer", {
		displayInLayerSwitcher : false
	});
	map.addLayer(vectors);

	box = new OpenLayers.Control.DrawFeature(vectors,
			OpenLayers.Handler.RegularPolygon, {
				handlerOptions : {
					sides : 4,
					snapAngle : 90,
					irregular : true,
					//persist : true
				}
			});
	
    box.handler.callbacks.done = endDrag;

    box.handler.callbacks.create = function(){vectors.removeAllFeatures();};
    
	map.addControl(box);

	box.activate();

	map.setCenter(lonlat, zoom);

}