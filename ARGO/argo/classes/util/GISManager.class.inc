<?php
include_once($_SERVER['DOCUMENT_ROOT']."/ARGO/classes/bean/GenericBean.class.inc");
	
class GISManager{
	
	static $containerStore = null;
	
	static function readGIS(){
		
			$containers = "[{'gid':'1','name':'Logística','radio':true},{'gid':'2','name':'Infraestrutura','radio':true},{'gid':'3','name':'Global','radio':true}]";
		
		
			$layers = "[ {'gid':'1','name':'ceara','title':'Municípios','url':'http://localhost:8080/geoserver/argo/wms','visibility':false,'group':'polygon','schema':'public','container':'Global','view':'false','consultable':false},
					     {'gid':'2','name':'bairro','title':'Bairro','url':'http://localhost:8080/geoserver/argo/wms','visibility':false,'group':'polygon','schema':'public','container':'Global','view':'false','consultable':false},
					     {'gid':'3','name':'new_via','title':'Ruas','url':'http://localhost:8080/geoserver/argo/wms','visibility':false,'group':'line','schema':'public','container':'Infraestrutura','view':'false','consultable':true},
						 {'gid':'4','name':'new_edge','title':'Trechos','url':'http://localhost:8080/geoserver/argo/wms','visibility':false,'group':'line','schema':'public','container':'Infraestrutura','view':'false','consultable':false},
						 {'gid':'5','name':'new_node','title':'Quinas','url':'http://localhost:8080/geoserver/argo/wms','visibility':false,'group':'point','schema':'public','container':'Infraestrutura','view':'false','consultable':false},
						 {'gid':'6','name':'euclidean_way','title':'Sequência','url':'http://localhost:8080/geoserver/argo/wms','visibility':false,'group':'line','schema':'public','container':'Logística','view':'false','consultable':false},
						 {'gid':'7','name':'dijkstran_way','title':'Rota','url':'http://localhost:8080/geoserver/argo/wms','visibility':false,'group':'line','schema':'public','container':'Logística','view':'false','consultable':false},
						 {'gid':'8','name':'kume','title':'Setor','url':'http://localhost:8080/geoserver/argo/wms','visibility':false,'group':'polygon','schema':'public','container':'','view':'false','consultable':false},
						 {'gid':'9','name':'cliente_ativo','title':'Pontos de Parada','url':'http://localhost:8080/geoserver/argo/wms','visibility':true,'group':'point','schema':'public','container':'Logística','view':'gid','consultable':true},
						 {'gid':'10','name':'clienten_900913','title':'Passageiros','url':'http://localhost:8080/geoserver/argo/wms','visibility':true,'group':'point','schema':'public','container':'Logística','view':'gid','consultable':true},
					]";
	
			$bounds ="-4600123.00177376,-872862.228945447,-4148725.90368004,-310694.972310875";
			
			$projection = "EPSG:900913";
			
			$zoom = '13';
			
			$center = '-4288700.58445,-415205.57338';
			
			HTML::generateJsScript("MapManager.projection = '$projection' ;MapManager.center =  new OpenLayers.LonLat($center),MapManager.bounds = new OpenLayers.Bounds($bounds);MapManager.zoom = $zoom; MapManager.layers = $layers; MapManager.containers = $containers; var allowFeature = false;");
			
			HTML::generateJsScript("MapManager.bairroStore = new Ext.data.ArrayStore({fields: ['gid','nome'],data : ".GenericDAO::retrieveBairros()."})");
			
			HTML::generateJsScript("MapManager.initGIS();");
		
	
	}
	
	static function makeLayers(){
		
		$list = GenericDAO::retrieveLayers(self::$containerStore);
		
		$layers = '[';
		
		$isFirst = true;
		
		foreach($list as $row){	

			foreach($row as $e){	
				if($isFirst){
					$layers .= "{'".$e['column']."':'".$e['value']."'";
					$isFirst = false;
				}
				else	
					$layers .= ",'".$e['column']."':'".$e['value']."'";
					
			}

			$layers .= ",'view':'". self::isViewLayer($row[6]['value'],$row[1]['value'])."'";
			
			$layers .= "},";
			
			$isFirst = true;		
			
		}
		
		$layers .= "]";
		
		return str_replace(":'t'", ":true",str_replace(":'f'", ":false", str_replace('},]', '}]', $layers)));
		
	}

	static function makeContainers($id){
		
		self::$containerStore = $list = GenericDAO::retrieveContainers($id);
		
		$containers = '[';
		
		$isFirst = true;
		
		foreach($list as $row){	

			foreach($row as $e){	
				if($isFirst){
					$containers  .= "{'".$e['column']."':'".$e['value']."'";
					$isFirst = false;
				}
				else	
					$containers  .= ",'".$e['column']."':'".$e['value']."'";
			
			}

			$containers  .= "},";
			$isFirst = true;		
					
		}
		
		$containers  .= "]";
		
		return str_replace(":'t'", ":true",str_replace(":'f'", ":false", str_replace('},]', '}]', $containers)));
		
	}
	
	static function isViewLayer($schema,$layer){
		
		$table = GenericDAO::checkViewForm($schema, $layer);
		
		if($table == $layer)
			return 'false';
		else 
			return GenericDAO::retrieveViewPrimaryKey($table);
		
	}
	
	static function hasClusters(){
	
		$rs = DatabaseUtil::execute("SELECT count(GID) FROM kume");
	
		while($row = pg_fetch_row($rs)){
	
			return ($row[0]);
	
		}
	
	}
}
?>