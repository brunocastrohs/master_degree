<!DOCTYPE html>
<html>
	<head>
		<title>GIS</title>
		<meta http-equiv="Content-Type" content="text/html;charset=UTF-8" />
	    
	    <?php 
			include_once($_SERVER['DOCUMENT_ROOT']."/ARGO/classes/util/GISManager.class.inc");
	
			GenericBean::ajaxifyMethods(array('retrieveGISColumns'));
			
			HTML::generateJsScript("var google = null;");
			
			//SVN - Printcapabilities
			HTML::generateJsScript("var printCapabilities = null;");
			
			HTML::generateJsScript("var geocode = null;");
			
			HTML::generateJsScript("var clusterNumber = ".GISManager::hasClusters().";");
			
		?>
	    <script src="http://maps.google.com/maps/api/js?v=3.2&amp;sensor=false"></script>
	    <script type="text/javascript" src="/ARGO/libs/extjs/ext-base.js"></script>
	    <script type="text/javascript" src="/ARGO/libs/extjs/ext-all.js"></script>
        <link rel="stylesheet" type="text/css" href="/ARGO/libs/extjs/resources/css/ext-all.css" />
		        <link rel="stylesheet" type="text/css" href="/ARGO/libs/extjs/resources/css/xtheme-gray.css" />

		<link rel="shortcut icon" href="/ARGO/images/favicon.ico"/>
		
		
		
        <script type="text/javascript" src="/ARGO/libs/openlayers/OpenLayers.js"></script>
		<link rel="stylesheet" type="text/css" href="/ARGO/libs/openlayers/theme/default/style.css"/>
        
        <script type="text/javascript" src="/ARGO/libs/geoext/GeoExt.js"></script>	
		<link rel="stylesheet" type="text/css" href="/ARGO/libs/geoext/resources/css/geoext-all.css"/>
		
		<script type="text/javascript" src="/ARGO/classes/ajax/MessageManager.js"></script>
		<script type="text/javascript" src="/ARGO/classes/ajax/MenuManager.js"></script>
		<script type="text/javascript" src="/ARGO/classes/ajax/MapManager.js"></script>
		<script type="text/javascript" src="/ARGO/classes/ajax/MapControls.js"></script>
		<script type="text/javascript" src="/ARGO/classes/ajax/MapAction.js"></script>
		<script type="text/javascript" src="/ARGO/classes/ajax/ActionManager.js"></script>
		<script type="text/javascript" src="/ARGO/classes/ajax/GeomManager.js"></script>
		<script type="text/javascript" src="/ARGO/classes/ajax/FormManager.js"></script>
		<script type="text/javascript" src="/ARGO/classes/ajax/TableGrid.js"></script>
		<link rel="stylesheet" type="text/css" href="/ARGO/styles/global.css"/>
		<link rel="stylesheet" type="text/css" href="/ARGO/styles/grid.css"/>
		<script type="text/javascript" src="/ARGO/libs/PHPLiveX/phplivex.js"></script>	
		
		<script type="text/javascript" src="http://localhost:8080/geoserver/pdf/info.json?var=printCapabilities"></script>
	</head>
	<body>
	<?php GISManager::readGIS(); ?>
	<script type="text/javascript">setInterval(MapAction.redrawLayers,1500);</script>
	<div id="script"></div>
	<div id="n"></div>
	<div id="here"></div> 	 
	<span id="pr"><img src="/ARGO/images/preloader.gif" alt="loading" /></span>	
	<script type="text/javascript">MapAction.initCluster();</script>
	</body>
</html>