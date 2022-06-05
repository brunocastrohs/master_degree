<?php

class DatabaseConstants{
	
	public static $foreignTables = array(
									'public.pluviometro'=>array('pluviometro.pluviometro_problema','pluviometro.historico_resolucao','pluviometro.historico_posicao'),
									'seguranca.usuario'=>array('seguranca.usuario_grupo'),
									'seguranca.grupo'=>array('seguranca.grupo_permissao'),
									'gis.projeto'=>array('gis.container'),
									'gis.container'=>array('gis.layer')
	);
	
	public static $tableViews = array(
						//		
			'public.cliente_ativo'	=>	'public.objeto',
			
			 
				);
	
	public static $labels = array(
			
			"cliente_ativo" => array(

					"gid" => 'Código do Cliente',
					"kume" => 'Setor',
					
			 )
		
	);
	
	public static $hiddens = array(
				
			"cliente_ativo" => array(
					"color"=>'',
					"subkume"=>'',
					"nome_setor"=>'',
			)
	
	);
	
	public static $selectHiddens = array(
	
			"cliente_ativo" => array(
					"color"=>'',
					"subkume"=>'',
					"kume"=>'',
					"ordem_visita"=>'',
	
			)
	
	);
	
	public static $subKumeTables = array(
	
			"objeto" => '',
			"cliente_ativo" => ''
	
	);
	
}

?>
