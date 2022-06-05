<?php 
header("Content-Type: text/html; charset=UTF-8");
class HTML{
	
	const INSERTED = "<div class=\"successMessage\">Dados cadastrados.</div>";
    const ALREADY_INSERTED = "<div class=\"warningMessage\">Dados já cadastrados.</div>";
    const WARNING = "<div class=\"warningMessage\">Usuário não cadastrado.</div>";
    const NOT_INSERTED = "<div class=\"errorMessage\">Dados não foram cadastrados!</div>";
    const REMOVED = "<div class=\"successMessage\">Dados removidos.</div>";
    const NOT_REMOVED = "<div class=\"errorMessage\">Dados não foram removidos!</div>";
    const UPDATED = "<div class=\"successMessage\">Dados atualizados.</div>";
    const NOT_UPDATED = "<div class=\"errorMessage\">Dados não foram atualizados!</div>";
	
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
			return "<TH style='width:$size%'>$value</TH>";
		else 
			return "<TD style='width:$size%'>$value</TD>";
		
	}
	
	static function createGeoCodColumn($value,$size,$flag){
	
		$value = self::checkGeoCodString($value);
		
		return "<TD class='geocodString' id='geocodString$flag' onclick='MapAction.geocodeTableString(\"$value\")' style='width:$size%'>$value</TD>";
		
	}
	
	static function checkGeoCodString($value){
	
		if(strstr($value, 'RU ')!=-1){
			
			return str_replace('RU ', 'RUA ', $value);
			
		}
		else if(strstr($value, 'TR ')!=-1){
			
			return str_replace('TR ', 'TRAVESSA ', $value);
			
		}
		else if(strstr($value, 'VL ')!=-1){
			
			return str_replace('VL ', 'VILA ', $value);
			
		}
	
	}
	
	static function createGeoColumn($value,$size){
		
		return "<TD style='width:$size%;'><img style='cursor:pointer;' src='/ARGO/images/search.gif' onclick=\"MapAction.setCenter('$value')\"/></TD>";
		
	}
	
	static function createReverseGeoCodColumn($value,$size){
	
		return "<TD style='width:$size%;'><img style='cursor:pointer;' src='/ARGO/images/search.gif' onclick=\"MapAction.reverseGeoCod('$value')\"/></TD>";
	
	}
	
}

?>