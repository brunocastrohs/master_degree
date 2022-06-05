MessageManager = {
		
	wdw:null,
		
	showMessage: function(text, type){
		
		if(type == 'success')
			MessageManager.successMessage(text);
		else if(type == 'error')
			MessageManager.errorMessage(text);
		else if(type == 'warning')
			MessageManager.warningMessage(text);
	},
	
	successMessage: function(text){
		MessageManager.wdw = new Ext.Window({ title : 'Aviso', html : text ,width:360, height:90});
		MessageManager.wdw.show();
		window.setTimeout('MessageManager.wdw.close()', 2000);
	},
	errorMessage: function(text){
		MessageManager.wdw = new Ext.Window({ title : 'Aviso', html : text ,width:360, height:90});
		MessageManager.wdw.show();
		window.setTimeout('MessageManager.wdw.close()', 2000);
	},
	warningMessage: function(text){
		MessageManager.wdw = new Ext.Window({ title : 'Aviso', html : text ,width:360, height:90});
		MessageManager.wdw.show();
		window.setTimeout('MessageManager.wdw.close()', 2000);
	}
		
};