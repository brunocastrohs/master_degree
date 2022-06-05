<?php

class HTML{

	const INSERTED = "<div class=\"successMessage\">Dados cadastrados com �xito.</div>";
	const ALREADY_INSERTED = "<div class=\"warningMessage\">Dados j� cadastrados.</div>";
	const WARNING = "<div class=\"warningMessage\">Usu�rio n�o cadastrado.</div>";
	const NOT_INSERTED = "<div class=\"errorMessage\">Dados n�o foram cadastrados!</div>";
	const REMOVED = "<div class=\"successMessage\">Dados removidos com �xito.</div>";
	const NOT_REMOVED = "<div class=\"errorMessage\">Dados n�o foram removidos!</div>";
	const UPDATED = "<div class=\"successMessage\">Dados atualizados com �xito.</div>";
	const NOT_UPDATED = "<div class=\"errorMessage\">Dados n�o foram atualizados!</div>";

	static  function createOption($value,$text){

		if($text == "")
		$option = '<option value="">Selecione...</option>';
			
		else
		$option = "<option value='$value'>$text</option>";
			
		return $option;


	}

	static  function createEnhancedOption($attributes,$value,$text){

		if($text == "")
		$option = '<option value="">Selecione...</option>';
			
		else
		$option = "<option $attributes value='$value'>$text</option>";
			
		return $option;


	}

	static function generateJsScript($code){

		echo "<script type='text/javascript'>$code</script>";

	}

	static function reload(){

		echo "<script type='text/javascript'>window.location.reload()</script>";

	}

	static function location($url){

		echo "<script type='text/javascript'>window.open('$url')</script>";

	}

	static function createJsScript($code){

		return  "<script type='text/javascript'>$code</script>";

	}

	static function createColumn($value,$size,$isHead){

		if($isHead)
		return "<TH style='width:100%'>$value</TH>";
		else
		return "<TD onclick ='ActionManager.sendLatLongRequest(\"$value\")' style='width:100%'>$value</TD>";

	}

	static function createGeoColumn($value,$size){

		$size = $size/2;

		return "<TD style='width:{$size}%'><img src='/GeoCod/images/search.gif' onclick=\"MapAction.setCenter('$value')\"/></TD>";

	}

}

?>