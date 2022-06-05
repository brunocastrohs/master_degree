<?php	

class ReportConstants{

	static $tableFontSize = 6;
	
	static $metaInformation = array ('Title'=>'PDF','Author'=>'Bruno Honorato','Subject'=>'','Creator'=>'brunonorato@hotmail.com','Producer'=>'GIS');
	
	static $reportsProperties = array();
	
	static function retrieveReports(){
		
		$reports = array();
		
		$reports[] = array('title'=>'Sequência de Visita','report'=>'sequencia'); 
		
		$reports[] = array('title'=>'Rota','report'=>'rota');
		
	//	$reports[] = array('title'=>'Histórico de Visitas','report'=>'visita');
		
		return $reports;
		
	}
	
	static function initReportsProperties(){
		
		self::$reportsProperties['sequencia'] = array(
		
		'title'=>'Sequência de Visita', 

		'description'=>'',
		
		'font'=>8,
				
		'cols' => array('indice'=>'Índice',"nome"=>'Cliente','endereco'=>'Endereço'),
		
		'tables' => "objeto, euclidean_sequence",

		'params' => "objeto_gid = objeto.gid ORDER BY indice"
		
		);
		
		self::$reportsProperties['rota'] = array(
		
		'title'=>'Rota', 

		'description'=>'',
		
		'font'=>8,

		'cols' => array('indice'=>'Índice','name'=>'Rua'),
		
		'tables' => "dijkstran_sequence, new_via",

		'params' => "edge_gid = new_via.gid ORDER BY indice"
		
		);
		
		
		
		
	}

}
?>