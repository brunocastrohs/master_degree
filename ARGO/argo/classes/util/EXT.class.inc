<?php
header("Content-Type: text/html; charset=UTF-8");
require_once($_SERVER['DOCUMENT_ROOT']."/ARGO/classes/util/HTML.class.inc");
require_once($_SERVER['DOCUMENT_ROOT']."/ARGO/classes/util/DateUtil.class.inc");


class EXT extends HTML{

	public static $extMenu;
	
	public static $extScrolls;

	public static $extScrollItens;
	
	public static $entityData;
	
	public static $entityDefaults;
	
	public static $entityColumns;
	
	public static $gridColumns;
	
	public static $gridToolBar;
	
	public static $idCount = 0;
	
	public static $schema = '';
	
	public static $table = '';
	
	public static $pk = '';
	
	public static $fks = null;
	
	public static $fkData = null;
	
	public static $comboStore = null;
	
	public static $defaultCombo = '';
	
	public static $defaultComboValue = '';
	
	public static $defaultComboTextValue = '';
	
	public static $hasGeom = 'false';
	
	public static $geomColumn = '';
	
	public static $geomType = '';
	
	public static $epsg = '';
	
	public static $isProject = false;
	
	public static $canEdit = '';
	
	public static $expandedColumn = '';
	
	public static $extFormFields = '';
	
	public static $extFormTool = '';
	
	public static $menuReports = array(); 
	
	public static $id = array();
	
	public static $view = null;

	public static $param = null;
	
	public static $hasSubKume = 'false';
	
	public static $kumeGid = 'false';
	//export columns
	
	static function generateColumnScreen($html){
	
		$schema = base64_encode(self::$schema);
	
		$table = base64_encode(self::$table);
	
		$param = base64_encode(self::$param);
			
		return "
		Ext.onReady(function() {
	
		FormManager.z = '$schema';
		FormManager.y = '$table';
		FormManager.x = '$param';
		
		
		ActionManager.north = new Ext.Toolbar({region:'north',height:28});
	
		ActionManager.form = {region:'center',height:(window.innerHeight-28),autoScroll: true,html:".'"'.$html.'"'."};
	
		ActionManager.north.add({text : 'PDF',iconCls : 'scrollReportItem', handler:function(){ActionManager.sendReportRequest('pdf');}});
		
		ActionManager.north.add({text : 'XLS',iconCls : 'menuXLSItem',handler:function(){ActionManager.sendReportRequest('xls');}});
		
		ActionManager.north.add({text : 'TXT',iconCls : 'menuTXTItem',handler:function(){ActionManager.sendReportRequest('txt');}});
	
		new Ext.Viewport({items: [ActionManager.north,ActionManager.form]});
	
		});";
		
	}
	//form
	static function generateForm(){
		
			$schema = base64_encode(self::$schema);
		
			$table = base64_encode(self::$table);
		
			$pk = base64_encode(self::$pk);
			
			//$id = base64_encode(self::$id);
						
			$view = self::$view;
		
			$fks = self::generateForeignArray();
		
			if($fks != "[]")
				$fkData=  implode(",",array_values(self::$fkData));
			else
				$fkData=  (self::$fkData);
			
			$hasGeom = self::$hasGeom;
			
			$geomColumn = self::$geomColumn;
		
			$geomType = self::$geomType;
			
			$formFields = self::$extFormFields;
			
			$formToolBar = self::$extFormTool;
			
			$epsg = self::$epsg;
			
			$hasSubKume = self::$hasSubKume;
			
			$kumeGid = "'".GenericDAO::$kumeGid."'";
			
			$subkume = GenericDAO::$hasSubkume?GenericDAO::$hasSubkume:"''";
			
		//	$top = strstr($view, 'posto')?"new Ext.Viewport({items: [ActionManager.north,ActionManager.form]});":"new Ext.Viewport({items: [ActionManager.form]});";
			
			return "
			Ext.onReady(function() {

    		FormManager.z = '$pk';
    		FormManager.x = '$schema';
    		FormManager.y = '$table';
			FormManager.o = '$view';
			FormManager.fs = $fks;
			FormManager.hasSubKume = $hasSubKume;
			FormManager.kume = $kumeGid;
			FormManager.subkume = $subkume;
		
			FormManager.fkData = { $fkData };
		
			GeomManager.hasGeom = $hasGeom;
			
			GeomManager.georg = '$geomColumn';
		
			GeomManager.type = '$geomType';
			
			GeomManager.epsg = '$epsg';
	
    		ActionManager.form = new Ext.FormPanel({autoScroll: true,height:(window.innerHeight-28),width:(window.innerWidth),region:'west',labelWidth: 120,frame:true,defaults: {width: 300},defaultType: 'textfield',items: $formFields,buttons: [$formToolBar]});
    		
    		new Ext.Viewport({items: [ActionManager.form]});
    		
			});";
			
	}
	
	static function generateInsertFormFields($list,$lat,$long){
		
		$extFormFields = '[';
		
		$isFirst = true;
		
		foreach($list as $object){

				if($isFirst){
					$extFormFields .= self::generateInsertFormField($object,$lat,$long);
					$isFirst = false;
				}
				else
					$extFormFields .= ','.self::generateInsertFormField($object,$lat,$long);
		
			//	if($object['type'] == 'fk' || $object['column'] == 'kume'){
			//		self::generateComboStore($object['column']);
			//	}
			}
		
		$extFormFields .= ']';
		
		self::$extFormFields = $extFormFields;
		
	}
	
	static function generateInsertFormField($object,$lat,$long){
		
		$hidden = '';
		
		$value = "''";
		
		$checked = '';
		
		$type = '';
		
		$geoFields = '';
		
		if(self::$pk == $object['column']){
			$value = self::$idCount;
		}
		else if($object['type'] == 'double precision' || $object['type'] == 'integer' || $object['type'] == 'numeric'){
			$value = '0';
			$type = "xtype:'numberfield',";
		}
		else if($object['type'] == 'date'){
			$value = "'01/01/2011'";
			$type = "xtype: 'datefield', format: 'd/m/Y',";
		}
		else if($object['type'] == 'boolean'){
			$value = 'true';
			$type = "xtype:'checkbox',";
			$checked = "checked:true,";
		}
		else if(strstr($object['type'], 'geometry')){
			
			self::$hasGeom = 'true';
			
			self::$geomColumn = $object['column']; 
			
			self::$geomType = GenericDAO::retrieveGeomType(self::$schema, self::$table);
			
			$value = '\'{"type":"'.self::$geomType.'","coordinates":[0,0]}\'';
			
			if(self::$geomType == 'POINT')
				$geoFields = ",{id:'x0',fieldLabel: 'Longitude(X)',name: 'x0',value:$long},{id:'y0',fieldLabel: 'Latitude(Y)',name: 'y0',value:$lat}";
		}
		else if($object['type'] == 'fk'){
			return '';
		}
		
		if(isset($object['isHidden']) || self::$hasGeom == 'true' || self::$pk == $object['column'] || strstr($object['type'], 'timestamp'))
			$hidden = ',hidden:true';
		
		$title = $object['label'];
		
		return "{ $type $checked id:'{$object['column']}',fieldLabel: '$title',name: '{$object['column']}',value:$value $hidden}$geoFields";
		
	}
	
	static function generateUpdateFormFields($records){
		
		$extFormFields = '[';
		
		$isFirst = true;
		
		foreach($records as $list){
			foreach($list as $object){

				if($isFirst){
					$extFormFields .= self::generateUpdateFormField($object);
					$isFirst = false;
				}
				else
					$extFormFields .= ','.self::generateUpdateFormField($object);
		
			//	if($object['type'] == 'fk' || $object['column'] == 'kume'){
			//		self::generateComboStore($object['column']);
			//	}
			}
		}
		
		
		if(isset(DatabaseConstants::$subKumeTables[self::$table])){
			if(GenericDAO::$hasSubkume){
				self::$hasSubKume = 'true';
				$extFormFields .= self::generateUpdateKumeFields();
			}
			$extFormFields .= ",{xtype:'fieldset',width:430,labelWidth:60,title: 'Restrição do Dia de Visita',collapsible: true,collapsed: true,  autoHeight:true,defaultType: 'textfield',items :[{ xtype:'checkbox', checked:".GenericDAO::$kumeDaysRestriction['1'].",  id:'restricao1',fieldLabel: 'Segunda',name: 'restricaoSegunda',value:'1'},{ xtype:'checkbox', checked:".GenericDAO::$kumeDaysRestriction['2'].",  id:'restricao2',fieldLabel: 'Terça',name: 'restricaoTerca',value:'2'},{ xtype:'checkbox', checked:".GenericDAO::$kumeDaysRestriction['3'].",  id:'restricao3',fieldLabel: 'Quarta',name: 'restricaoQuarta',value:'3'},{ xtype:'checkbox', checked:".GenericDAO::$kumeDaysRestriction['4'].",  id:'restricao4',fieldLabel: 'Quinta',name: 'restricaoQuinta',value:'4'},{ xtype:'checkbox', checked:".GenericDAO::$kumeDaysRestriction['5'].",  id:'restricao5',fieldLabel: 'Sexta',name: 'restricaoSexta',value:'5'}]}";
		}
		$extFormFields .= ']';
		
		self::$extFormFields = $extFormFields;
		
	}
	
	
	//TYPE
	static function generateUpdateFormField($object){
		
		$hidden = '';
		
		$checked = '';
		
		$type = '';
		
		$geoFields = '';
			
		if(is_numeric($object['value']) && ($object['column'] != 'pst_ativo' && $object['column'] != 'mit_log_resolvido')){
			$value = "{$object['value']}";
			$type = "xtype:'numberfield',";
		}
		else if($object['type'] == 'date'){
			$object['value']  = DateUtil::dateToApp($object['value']);
			$type = "xtype: 'datefield', format: 'd/m/Y',";
		}
		else if($object['value'] == "t" || (($object['column'] == 'pst_ativo' && $object['value'] == '1') ||($object['column'] == 'mit_log_resolvido' && $object['value'] == '1') )){
			$object['value'] = 'true';
			$type = "xtype:'checkbox',";
			$checked = "checked:true,";
		}
		else if($object['value'] == "f" || (($object['column'] == 'pst_ativo' && $object['value'] == '0') || ($object['column'] == 'mit_log_resolvido' && $object['value'] == '0') )){
			$object['value'] = 'false';
			$type = "xtype:'checkbox',";
			$checked = "checked:false,";
		}
		else if(strstr($object['type'], 'geometry')){
			
			self::$hasGeom = 'true';
			
			self::$geomColumn = $object['column'];
			
			self::$geomType = GenericDAO::retrieveGeomType(self::$schema, self::$table);
			
			$geoJson = (array)json_decode($object['value']);
			
			$object['value'] = "'".str_replace('"', '\"', $object['value'])."'";
			
			if(self::$geomType == 'POINT')
				$geoFields = ",{id:'x0',fieldLabel: 'Longitude(X)',name: 'x0',value:".$geoJson['coordinates'][0]."},{id:'y0',fieldLabel: 'Latitude(Y)',name: 'y0',value:".$geoJson['coordinates'][1]."}";
		}
		else if($object['type'] == 'fk'){
			
			$defaultValueText = "valueNotFoundText:'{$object['value']}' ,";
					
			return "new Ext.form.ComboBox({id:'{$object['column']}',fieldLabel: '{$object['label']}',displayField:'label',valueField:'{$object['column']}',typeAhead: true,mode: 'local',width: 210,triggerAction: 'all',$defaultValueText  emptyText: '{$object['value']}',selectOnFocus:true,store: new Ext.data.ArrayStore({fields: ['{$object['column']}', 'label'],data : [".self::generateComboStore($object['column'])."]}) })";
		}
		else
			$object['value'] = '"'.str_replace('"', '\"', $object['value']).'"';
			
		if(isset($object['isHidden']) || self::$hasGeom == 'true')
			$hidden = ',hidden:true';
		
		$title = $object['label'];
		
		return "{ $type $checked  id:'{$object['column']}',fieldLabel: '$title',name: '{$object['column']}',value:{$object['value']} $hidden} $geoFields";
		
	}
		
	static function generateUpdateKumeFields(){
	
		$extFormFields = ",{xtype:'fieldset',width:430,labelWidth:130,title: 'Dias do Atendimento',collapsible: true,collapsed: true,  autoHeight:true,defaultType: 'textfield',items :[";
	
		$isFirst = true;
	
		foreach(GenericDAO::$kumeDays as $object){
		
				if($isFirst){
					$extFormFields .= "{ xtype:'checkbox', checked: {$object['checked']},  id:'atend{$object['gid']}',fieldLabel: '{$object['descricao']}',name: 'atend{$object['gid']}',value:'{$object['gid']}'}";
					$isFirst = false;
				}
				else
					$extFormFields .= ','."{ xtype:'checkbox', checked: {$object['checked']},  id:'atend{$object['gid']}',fieldLabel: '{$object['descricao']}',name: 'atend{$object['gid']}',value:'{$object['gid']}'}";
	
				//	if($object['type'] == 'fk' || $object['column'] == 'kume'){
				//		self::generateComboStore($object['column']);
				//	}
		
		}

		$extFormFields .= ']}';
	
		return $extFormFields;
	
	}
	
	static function generateFormToolBar($action){
		
		$saveButton = "{text: 'Salvar',iconCls : 'saveButton',handler: function(btn) {
        		ActionManager.sendForm('insert','here');
            }
        	}";
		
		$updateButton = "{text: 'Salvar',iconCls : 'saveButton',handler: function(btn) {
        	   	ActionManager.sendForm('update','here');
            }
        	}";
		
		$removeButton = "{text: 'Remover',iconCls : 'removeButton',handler: function(btn) {
            	ActionManager.sendForm('remove','here');
            }
        	}";
		
		if(SecurityManager::checkPermission('EDITAR_'.strtoupper(self::$table)) && $action != ''){
			
			self::$extFormTool = "$updateButton";
			
			if(SecurityManager::checkPermission('REMOVER_'.strtoupper(self::$table)))
					self::$extFormTool .= ", $removeButton";
		}
		else if(SecurityManager::checkPermission('INSERIR_'.strtoupper(self::$table)))
			self::$extFormTool = $saveButton;
		
		
	}

	//loginbar
	static function generateLoginBar(){
		
			return "
			Ext.onReady(function() {
			
			MenuManager.north = new Ext.Toolbar({region:'north',height:50});
		
			MenuManager.north.add(MenuManager.logo, MenuManager.mover,
					'Usuário:', new Ext.form.TextField({id: 'login'}), MenuManager.separator, 
					'Senha:', new Ext.form.TextField({id: 'pass',inputType:'password'}), MenuManager.separator, 
					{text : 'Login',iconCls : 'menuItemLogin',handler : MenuManager.onLoginButtonClick}

			);
			
			MenuManager.center = new Ext.TabPanel({region : 'center',deferredRender : false,activeTab: 0});
			
			MenuManager.center.add({
			title : 'GIS',
			autoScroll : true,
			closable : false,
			html : '<iframe class=\"page\" src=\"/ARGO/classes/receiver/GenericReceiver.php?action='+'project'+'\"></iframe>'
			});
			
			new Ext.Viewport({layout : 'border',items : [ MenuManager.north, MenuManager.center ]});
			
			});";
			
	}
	
	static function generateLoginPage(){
		
			return "
			Ext.onReady(function() {
	
	 		var bd = Ext.getBody();

			ActionManager.form = new Ext.FormPanel({
			labelWidth: 75,frame:true,width: 350,defaults: {width: 230},defaultType: 'textfield',
			items: [
       		{id:'login',fieldLabel: 'Usuário',name: 'usuario',value:''},
       		{id:'pass',	fieldLabel: 'Senha',name: 'senha',value:'',inputType:'password'}
			],
			buttons: [{text: 'Login',iconCls : 'menuItemLogin',handler : MenuManager.onLoginButtonClick
			}]
			});

			ActionManager.form.render(document.body);
	
			});"
			;
			
	}
	
	//toolbar
	static function generateScrolls($list){
		
		$extScrolls = '{';
		
		$isFirst = true;
		
		foreach($list as $object){

			if($isFirst){
				$extScrolls .= "'".$object['schema']."':new Ext.menu.Menu()";
				$isFirst = false;
			}
			else
				$extScrolls .= ",'".$object['schema']."':new Ext.menu.Menu()";
	
		}
		
		$extScrolls .= ",'reports':new Ext.menu.Menu()";
		
		$extScrolls .= '}';
		
		self::$extScrolls = $extScrolls;
		
	}
	
	static function generateScrollMenuItens($list){
		
		$extScrollItens = '';
		
		$isFirst = true;
		
		foreach($list as $object)
				$extScrollItens .= self::generateScrollMenuItem($object['schema'], $object['table'],$object['label']);
		
		foreach(self::$menuReports as $report)
				$extScrollItens .= self::generateReportScrollMenuItem($report['title'], $report['report']);
				
		self::$extScrollItens = $extScrollItens;
		
	}
	
	static function generateScrollMenuItem($schema,$table,$label){
		
		$schema_e = base64_encode($schema);
		
		$table_e = base64_encode($table);
		
		return "scrollMenu['$schema'].add({
				iconCls:'scrollItem',
				x:'$schema_e',
				y:'$table_e',
				text : '$label',
				handler : MenuManager.onButtonClick
			});";

	}
	
	static function generateReportScrollMenuItem($title,$report){
		
		$report = base64_encode($report);
		
		return "scrollMenu['reports'].add({
				iconCls:'scrollReportItem',
				report:'$report',
				text : '$title',
				handler : MenuManager.onReportButtonClick
			});";

	}
	
	static function generateMenu($list){
		
		$extMenu = '';
		
		$isFirst = true;
		
		foreach($list as $object){

			if($isFirst){
				$extMenu .= self::generateMenuItem($object['schema']);
				$isFirst = false;
			}
			else
				$extMenu .= ',MenuManager.separator,'.self::generateMenuItem($object['schema']);
	
		}

		$extMenu .= ",MenuManager.separator,{text : 'Relatórios',iconCls : 'menuReportItem',menu : scrollMenu['reports']}";
		
		$extMenu .= ",MenuManager.separator,{text : 'Logout',iconCls : 'menuItemLogout',handler : MenuManager.onLogoutButtonClick}";
		
		self::$extMenu = $extMenu;
		
	}
	
	static function generateMenuItem($schema){
		
		$label = str_replace("_", " ", $schema);
		
		if($label == 'public')
			$label = 'layers';
		
		return "{text : '".ucwords($label)."',iconCls : 'menuItem',menu : scrollMenu['$schema']}";
		
	}
	
	static function generateToolBar(){
		
		$extMenu = self::$extMenu;
	
		$extScrolls = self::$extScrolls;

		$extScrollItens = self::$extScrollItens;
		
		
		$toolBarSource = "
			Ext.onReady(function() {
			
			MenuManager.north = new Ext.Toolbar({region:'north',height:50});
		
			MenuManager.center = new Ext.TabPanel({region : 'center',deferredRender : false,activeTab: 0});
			
			MenuManager.center.add({
			title : 'GIS',
			autoScroll : true,
			closable : false,
			html : '<iframe class=\"page\" src=\"/ARGO/pages/GISAPP/\"></iframe>'
			});
			
			var scrollMenu = $extScrolls;

			$extScrollItens
			
			MenuManager.north.add(MenuManager.logo, MenuManager.mover, $extMenu);

			new Ext.Viewport({layout : 'border',items : [ MenuManager.north, MenuManager.center ]});
			
		});";
		
		return $toolBarSource;
	}

	//end toolbar
	static function generateEntitiesData($records,$isRecord){
		
		$extEntityData = '[';
		//new Ext.data.Record({id:0,start : s,name : 'Luchio',email : 'ludmil@exttest.com',active : true,salary : 1000});
		$record = array('left'=>'','right'=>'');
		
		if($isRecord){
			$record['left'] = 'new Ext.data.Record(';
			$record['right'] = ')';		
		}
		
		$isFirst = true;
		
		$isFirstEntity = true;
		
		foreach($records as $list){
			
			if($isFirst){
				$extEntityData .= $record['left']."{";
				$isFirst = false;
			}
			else
				$extEntityData .=",".$record['left']."{";
			
			foreach($list as $object){

				if($object['type'] != 'action')
					if($isFirstEntity){
						$extEntityData .= self::generateEntityData($object);
						$isFirstEntity = false;
					}
					else
						$extEntityData .= ','.self::generateEntityData($object);
	
			}
			
			$isFirstEntity = true;
			
			$extEntityData .="}".$record['right'];
		
		}
		
		$extEntityData .= ']';
		
		self::$entityData = $extEntityData;
		
	}
	
	static function generateEntityData($object){
	
		$value = self::checkEntityData($object);
			
		return "{$object['column']}:$value";
		
	}
	//TYPE
	static function checkEntityData($object){

		if(is_numeric($object['value']))
			$object['value'] = "{$object['value']}";
		else if($object['type'] == 'date')
			$object['value']  = DateUtil::dateToApp($object['value']);
		else if($object['value'] == "t")
			$object['value'] = 'true';
		else if($object['value'] == "f")
			$object['value'] = 'false';
		else if(strstr($object['type'], 'geometry')){
			
			self::$hasGeom = 'true';
			
			self::$geomColumn = $object['column'];
			
			$object['value'] = "'".str_replace('"', '\"', $object['value'])."'";
		}
		else
			$object['value'] = '"'.str_replace('"', '\"', $object['value']).'"';
		
		return $object['value'];
		
	}
	
	static function generateDefaultEntities($list){
		
		$extEntity = '{';
		
		$isFirst = true;
		
		foreach($list as $object){
			if($object['type'] != 'action')
				if($isFirst){
					$extEntity .= self::generateDefaultEntity($object['column'], $object['type']);
					$isFirst = false;
				}
				else
					$extEntity .= ','.self::generateDefaultEntity($object['column'], $object['type']);
	
		}
		
		$extEntity .= '}';
		
		self::$entityDefaults = $extEntity;
		
	}
	//TYPE
	static function generateDefaultEntity($column,$type){
		
		$value = "''";
		
		if($column == self::$pk){
			$value = 'idCount';
		}
		else if($type == 'double precision' || $type == 'integer' || $type == 'numeric'){
			$value = '0';
		}
		else if($type == 'date'){
			$value = "'".date("d/m/Y")."'";
		}
		else if($type == 'boolean'){
			$value = 'true';
		}
		else if(strstr($type, 'geometry')){
			$value = '{"type":"'.self::$geomType.'","coordinates":[0,0]}';
		}
		
		return "$column: $value";
		
	}
	
	static function generateEntityColumns($list){
		
		$extEntityColumn = '[';
		
		$isFirst = true;
		
		foreach($list as $object){

			if($isFirst){
				$extEntityColumn .= self::generateEntityColumn($object['column'], $object['type']);
				$isFirst = false;
			}
			else
				$extEntityColumn .= ','.self::generateEntityColumn($object['column'], $object['type']);
	
		}
		
		$extEntityColumn .= ']';
		
		self::$entityColumns = $extEntityColumn;
	}
	//TYPE
	static function generateEntityColumn($column,$type){
		
		if($type == 'date' || $type == 'text' || $type == 'fk' || strstr($type, 'geometry') || strstr($type, 'character'))
			$type = 'string';
		else if($type == 'boolean')
			$type = 'bool';
		else if($type == 'action')
			return "";
			
		return "{name:'$column',type:'$type'}";
		
	}
	
	static function generateGridColumns($list){
	
		$extGridColumn = '[';
		
		$isFirst = true;
		
		foreach($list as $object){

			if($isFirst){
				$extGridColumn .= self::generateGridColumn($object);
				$isFirst = false;
			}
			else
				$extGridColumn .= ','.self::generateGridColumn($object);
		
			if($object['type'] == 'fk'){
				
				self::generateComboStore($object['column']);
				
			}
		
		}
		
		$extGridColumn .= ']';
		
		self::$gridColumns = $extGridColumn;		
	}
		
	static function generateGridColumn($object){
		
		//labeling
		$title = $object['label'];
		
		$hidden = isset($object['isHidden'])?"hidden:true,":'';
		
		self::$expandedColumn = isset($object['isExpanded'])?$object['column']:'';
		
		if(isset($object['isExpanded']))
			HTML::generateJsScript("GridVariables.expanded = '{$object['column']}';");
		
		$fieldType = self::getFieldType($object);
		
		$columType = self::getGridColumnType($object);
		
		return "{ $hidden id:'{$object['column']}',header: '$title', sortable: true $fieldType $columType}";
		
	}
	//TYPE
	static function getFieldType($object){
	
		if($object['type'] == 'boolean')
			return ",dataIndex: '{$object['column']}',editor: {xtype: 'checkbox' ,allowBlank: true}";
		else if($object['type'] == 'double precision' || $object['type'] == 'integer' || $object['type'] == 'numeric')
			return ",dataIndex: '{$object['column']}',editor: {xtype: 'numberfield' ,allowBlank: true, emptyText:'{$object['column']}...'}";
		else if($object['type'] == 'text')
			return ",dataIndex: '{$object['column']}',editor: {xtype: 'textarea' ,allowBlank: true, emptyText:'{$object['column']}...'}";
		else if($object['type'] == 'fk'){
			$defaultValueText = isset($object['defaultValueText'])?"valueNotFoundText:'{$object['defaultValueText']}' ,":'';
			return ",dataIndex: '{$object['column']}',editor: {xtype:'combo',typeAhead: true,triggerAction: 'all',id:'{$object['column']}',mode: 'local', $defaultValueText emptyText: 'Selecionar {$object['column']}...',displayField:'label',valueField: '{$object['column']}',store: new Ext.data.SimpleStore({id: 0,fields: ['{$object['column']}','label'],data: [".self::generateComboStore($object['column'])."]})}";
		}
		else if($object['type'] == 'action'){
			return "";
		}
		else
			return ",dataIndex: '{$object['column']}',editor: {xtype:'textfield', emptyText:'{$object['column']}...'}";
		
	}
	
	//TYPE
	static function getGridColumnType($object){
		
		if($object['type'] == 'boolean')
			return " ,xtype: 'booleancolumn',align: 'center',trueText: 'Sim',falseText: 'Não'";
		else if($object['type'] == 'double precision')
			return " ,xtype: 'numbercolumn',format: '0,0.00'";
		else if($object['type'] == 'integer')
			return " ,xtype: 'numbercolumn',format:'0'";
		else if($object['type'] == 'date')
			return " ,format: 'd/m/Y'";
		else if($object['type'] == 'action')
			return " ,xtype : 'actioncolumn',width : 120,items :[{icon : '/ARGO/images/scroll_item.gif',tooltip : 'View report',handler : function(grid, rowIndex,colIndex) {var rec = GridVariables.store.getAt(rowIndex);ActionManager.openWindow('/ARGO/classes/receiver/GenericReceiver.php?action=gridForeignForm&x={$object['foreignSchema']}&y={$object['foreignTable']}&z={$object['foreignColumn']}&id='+rec.get('".self::$pk."'));} }]";
		else
			return '';
		
	}
	
	static function generateComboStore($column){
		
		$isFirst = true;
		
		$data = '';
		
		foreach(self::$fks as $fk){
			if($fk['fk_name'] == $column)
				foreach($fk['fk_records'] as $d)
					if($isFirst){
						$data .= '["'.$d['id'].'","'.$d['value'].'"]';
						$isFirst = false;
					}
					else 
						$data .= ',["'.$d['id'].'","'.$d['value'].'"]';
		}
		
		self::$fkData["$column"] = "'$column':[$data]";
		
		return $data;
		
	}
	
	static function generateForeignArray(){
		
		$isFirst = true;
		
		if(!self::$fks)
			return "[]";
		
		$data = '[';
		
		foreach(self::$fks as $fk){
					if($isFirst){
						$data .= '"'.$fk['fk_name'].'"';
						$isFirst = false;
					}
					else 
						$data .= ',"'.$fk['fk_name'].'"';
		}
		
		$data .= ']';
		
		return $data;
		
	}
	
	static function generateGridToolBar(){
	
		$addButton = "{iconCls: 'gridAdd',text: 'Adicionar',
        handler: function(){
        	ActionManager.isInsert = true;
        	idCount++;
            var e = new Entity(".self::$entityDefaults.");
            
            if(GeomManager.hasGeom)
				GeomManager.clearGeomForm();
			
            GridVariables.editor.stopEditing();
            GridVariables.store.insert(0, e);
            GridVariables.grid.getView().refresh();
            GridVariables.grid.getSelectionModel().selectRow(0);
            GridVariables.editor.startEditing(0);
        }
    	}";
		
		$removeButton = "{ref:'../removeBtn',iconCls: 'gridRemove',text: 'Remover',disabled: true,
        handler: function(){
            GridVariables.editor.stopEditing();
            var s = GridVariables.grid.getSelectionModel().getSelections();
            for(var i = 0, r; r = s[i]; i++){
                GridVariables.store.remove(r);
                ActionManager.sendRequest(r.data,'remove','here');
            }
        }
    	}";
		
		$updateButton = "{iconCls: 'gridUpdate',text: 'Atualizar',
        handler: function(){
        window.location.reload();
    	}}";
		
		$gridToolBar = '';
		
		/*
		if(!self::$isProject && SecurityManager::checkPermission('INSERIR_'.strtoupper(self::$table))) {
			$gridToolBar .= "$addButton,";
		}
		if(!self::$isProject && SecurityManager::checkPermission('REMOVER_'.strtoupper(self::$table))){
			$gridToolBar .= "$removeButton,";
		}
		if(!self::$isProject && !SecurityManager::checkPermission('EDITAR_'.strtoupper(self::$table))){
			self::$canEdit = '//';
		}
		*/
		$gridToolBar .= "$updateButton";
			
		self::$gridToolBar = $gridToolBar;		
	}
	
	//project grid
	//statics gid
	static function generateProjectGridColumns($list){
	
		$extGridColumn = "[
		{id: 'pro_codigo',header: 'Gid',dataIndex: 'pro_codigo', sortable: true,editor: {xtype: 'numberfield' ,allowBlank: false}  ,xtype: 'numbercolumn',format:'0',hidden:true },
		{id: 'pro_nome',header: 'Nome',dataIndex: 'pro_nome', sortable: true,editor: {xtype: 'textfield' ,allowBlank: false} },
		{id: 'pro_descricao',header: 'Descricao',width:450,dataIndex: 'pro_descricao', sortable: true,editor: {xtype: 'textfield' ,allowBlank: false},hidden:true },
		{id: 'pro_data',header: 'Data',dataIndex: 'pro_data', sortable: true,editor: {xtype: 'textfield' ,allowBlank: false}  ,format: 'd/m/Y'},
		{header : 'Descrição',xtype : 'actioncolumn',width : 70,items :[{icon : '/ARGO/images/description.gif',tooltip : 'View report',
		handler : function(grid, rowIndex,colIndex) {
		var r = GridVariables.store.getAt(rowIndex);
		var win = new Ext.Window({title : 'Descrição',width : 600,height:400,autoScroll : true,items : [ {html : '<textarea style=\"width:99%; height:99%; border:0px;\">'+r.data['pro_descricao'].replace('. ', '.\\n')+'</textarea>'} ]});			
		win.show();
		}
		}]},
		{header : 'Relatório',xtype : 'actioncolumn',width : 70,items :[{icon : '/ARGO/images/report.gif',tooltip : 'View report',handler : function(grid, rowIndex,colIndex) {var rec = GridVariables.store.getAt(rowIndex);window.open('/ARGO/tmp/'+rec.get('pro_relatorio')+'.pdf');} }]},
		{header : 'GIS',xtype : 'actioncolumn',width : 50,items :[{icon : '/ARGO/images/gis.gif',tooltip : 'View GIS',
		handler : function(grid, rowIndex,colIndex) {
			var rec = GridVariables.store.getAt(rowIndex);
			window.open('/ARGO/pages/GISAPP/index.php?gid='+ rec.get('pro_codigo')+'&zoom='+ rec.get('pro_zoom')+'&limite='+ rec.get('pro_limite')+'&projecao='+ rec.get('pro_projecao')+'&nome='+rec.get('pro_nome')+'&center='+rec.get('pro_center'));
		}
		}]},
		{id: 'pro_relatorio',dataIndex: 'pro_relatorio', sortable: true,editor: {xtype: 'textfield' ,allowBlank: false},hidden:true },
		{id: 'pro_zoom',dataIndex: 'pro_zoom', sortable: true,editor: {xtype: 'textfield' ,allowBlank: false},hidden:true },
		{id: 'pro_limite',dataIndex: 'pro_limite', sortable: true,editor: {xtype: 'textfield' ,allowBlank: false},hidden:true },
		{id: 'pro_center',dataIndex: 'pro_center', sortable: true,editor: {xtype: 'textfield' ,allowBlank: false},hidden:true },
		{id: 'pro_projecao',dataIndex: 'pro_projecao', sortable: true,editor: {xtype: 'textfield' ,allowBlank: false},hidden:true }
		]";
		
		self::$canEdit = '//';
		
		self::$gridColumns = $extGridColumn;		
	}

	static function generateRowGrid(){
	
		//GridPage
		$entityData = self::$entityData;
	
		$entityDefaults = self::$entityDefaults;
	
		$entityColumns = self::$entityColumns;
	
		$gridColumns = self::$gridColumns;
		
		$gridToolBar = self::$gridToolBar;
		
		$idCount = self::$idCount;
		
		//FormManager
		$schema = self::$schema;
		
		$table = self::$table;
		
		//if($table == 'projeto')
			//HTML::generateJsScript("GridVariables.expanded = 'pro_nome'");
		
		$pk = self::$pk;
		
		$fks = self::generateForeignArray();
		
		$fkData = '';
		
		if(self::$fkData)
			$fkData = implode(",",array_values(self::$fkData));
		
		//GeomManger
		$hasGeom = self::$hasGeom;
		
		$geomColumn = self::$geomColumn;
		
		$geomType = self::$geomType;
		
		$epsg = self::$epsg;
		
		//comboboxes(FormManager)
		$defaultCombo = self::$defaultCombo;
			
		$defaultComboValue = self::$defaultComboValue;
	
	    $defaultComboTextValue = self::$defaultComboTextValue;

	    //GridRow
		$east = '';
		
		if($hasGeom == 'true')
			$east = ',east';
		
		$canEdit = self::$canEdit;
		
		$gridSource = 
		" 
		FormManager.x = '".base64_encode($schema)."';
		
		FormManager.y = '".base64_encode($table)."';
		
		FormManager.z = '".base64_encode($pk)."';
		
		FormManager.fs = $fks;
		
		FormManager.fkData = { $fkData };
		
		FormManager.defaultCombo = '$defaultCombo';
		
		FormManager.defaultComboValue = '$defaultComboValue';
		
		FormManager.defaultComboTextValue = '$defaultComboTextValue';
		
		GeomManager.hasGeom = $hasGeom;
		
		GeomManager.georg = '$geomColumn';
		
		GeomManager.type = '$geomType';
		
		GeomManager.epsg = '$epsg';
		
		Ext.onReady(function(){
	
		GridVariables.editor = new Ext.ux.grid.RowEditor({
	        saveText: 'Salvar'
	    });

    	var entityColumns = $entityColumns;
    
    	var Entity = Ext.data.Record.create(entityColumns);
  
    	var entityData = $entityData;
    
    	var idCount = $idCount;
    	
    	GridVariables.store = new Ext.data.GroupingStore({
        reader: new Ext.data.JsonReader({fields: Entity}),
        data:entityData
	    	});

    	var gridColumns = $gridColumns;
    
    	var gridToolbar = [$gridToolBar];
    	
    	GridVariables.grid = new Ext.grid.GridPanel({
    	region:'center',
        store: GridVariables.store,
        region:'center',
        margins: '0 5 5 5',
        autoExpandColumn:GridVariables.expanded,
       $canEdit plugins: [GridVariables.editor],
        view: new Ext.grid.GroupingView({
            markDirty: false
        }),
        tbar: gridToolbar,
        columns: gridColumns    
        });

        var west = {
 			region: 'west',
 			id:'geos',
 			autoScroll: true,
            title: 'Consulta',
            split: true,
            width: 220,
            minSize: 175,
            maxSize: 400,
            collapsible: true,
            collapsed:true,
            margins: '0 0 0 5',
            layout: {type: 'accordion',animate: true},
            autoLoad: {url:'/ARGO/pages/Common/QueryForm.php?x='+FormManager.x+'&y='+FormManager.y,scripts:false}
		};
        
		if(GeomManager.hasGeom)
		var east = {
			region : 'east',			
			autoScroll : true,
			title : 'Geometria:$geomType',
			split : true,
			width : 260,
			minSize : 175,
			maxSize : 400,
			collapsible : true,
			margins : '0 0 0 5',
			layout : {
				type : 'accordion',
				animate : true
			},
			autoLoad : {
				url : '/ARGO/pages/Common/GeomForm.php'
			}
		};
		
		
    	new Ext.Viewport({layout: 'border',items: [GridVariables.grid,west $east]});
    
    	GridVariables.grid.getSelectionModel().on('selectionchange', function(sm){
		if(GridVariables.grid.removeBtn)
    	GridVariables.grid.removeBtn.setDisabled(sm.getCount() < 1);
    	});
		});";

		return $gridSource;
	
	}
	
}

?>