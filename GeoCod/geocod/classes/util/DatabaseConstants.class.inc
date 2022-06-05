<?php

class DatabaseConstants{

	public static $hiddenSchemas = array('topology');

	public static $hiddenTables = array('spatial_ref_sys','raster_columns','raster_overviews');

	public static $constraintHiddenTables = "'gis.project','public.pluviometro','public.espelhos_dagua'";

	public static $constraintHiddenFields = array('public.pluviometro'=>'intervalo');

	public static $foreignTables = array(
									'public.pluviometro'=>array('pluviometro.pluviometro_problema','pluviometro.historico'),
									'gis.project'=>array('gis.container'),
									'gis.container'=>array('gis.layer')

	);

	public static $tableViews = array(
									 'public.espelhos_dagua_s'		=>	'espelhos_dagua_brasil',
									 'public.pluviometro_ativo'		=>	'pluviometro',
									 'public.pluviometro_inativo'	=>	'pluviometro'
									 );

}

?>