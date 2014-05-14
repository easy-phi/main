<!--
Easy-phi project: http://easy-phi.ch/
Developer : Raphael Houlmann, University of Geneva  2014
Licensed under GPL, for details report to: http://easy-phi.ch/index.php/about/licensing
Do NOT remove this notice
-->
<link rel="stylesheet" href="/jquery/css/ui-lightness/jquery-ui-1.10.3.custom.css" />
<script src="/jquery/js/jquery-1.9.1.js"></script>
<script src="/jquery/js/jquery-ui-1.10.3.custom.js"></script>
<script src="/js/util.js"></script>
<link rel="stylesheet" href="/css/easy-phi-custom.css" />

<html>
<head>
          
  <script lang="text/javascript">
  $(document).ready(function(){
    <?php print("var port ="); print ($_GET["s"]+5555); ?>          
    
    function request_serial(callback, text){
        $.ajax({
            url: "/php/easy-phi.php?p=" + port + "&q=" + text,              
            async: true,
            success : function(data){callback(data);}
        });     
    }
    
    function none(data){
    }

    function write_to_box(data){
        $('#mqueuewindow').append(data + '<br>');
        $('#mqueuewindow').scrollTop($('#mqueuewindow')[0].scrollHeight);    
    }
    
    
  	$('#endela').change(function() {
        if ($(this).is(':checked')) {
            request_serial(none, ("CONFigure:DELay:Reset:ENable 1")); 
        } else {
            request_serial(none, ("CONFigure:DELay:Reset:ENable 0")); 
        }        
    });     

  	$('#endelb').change(function() {
        if ($(this).is(':checked')) {
            request_serial(none, ("CONFigure:DELay:Data:ENable 1")); 
        } else {
            request_serial(none, ("CONFigure:DELay:Data:ENable 0")); 
        }        
    });     

  	$('#endelc').change(function() {
        if ($(this).is(':checked')) {
            request_serial(none, ("CONFigure:DELay:Clock:ENable 1")); 
        } else {
            request_serial(none, ("CONFigure:DELay:Clock:ENable 0")); 
        }        
    });     

    
	$('#delaya').spinner({min: 0, max: 11242, step: 1, increment: 'fast', stop: function(event, ui) {
        var n = ($(this).val());
        if (isInteger(n)) {
            request_serial(none, ("CONFigure:DELay:Reset:VALue " + (parseInt(n)))); 
        }    
        }    
    });     
    
	$('#delayb').spinner({min: 0, max: 11242, step: 1, increment: 'fast', stop: function(event, ui) {
        var n = ($(this).val());
        if (isInteger(n)) {
            request_serial(none, ("CONFigure:DELay:Data:VALue " + (parseInt(n)))); 
        }    
        }    
    });     

	$('#delayc').spinner({min: 0, max: 11242, step: 1, increment: 'fast', stop: function(event, ui) {
        var n = ($(this).val());
        if (isInteger(n)) {
            request_serial(none, ("CONFigure:DELay:Clock:VALue " + (parseInt(n)))); 
        }    
        }    
    });     

	$('#attenuation').spinner({min: 0, max: 31.75, step: 0.01, increment: 'fast', stop: function(event, ui) {
        var n = ($(this).val());
        if (isFloat(n)) {
            request_serial(none, ("CONFigure:RF:ATTenuation " + (parseFloat(n)))); 
        }    
        }    
    });     

    
  	$('#mqueueform').button().click(function(e) {
        request_serial(write_to_box, $('#mqueuebox').val()); 
  		e.stopPropagation();
  		e.preventDefault();
  	});
/// initialisation    
    function setenablea(data){if (data==1){$('#endela').prop('checked', true);}};
    function setenableb(data){if (data==1){$('#endelb').prop('checked', true);}};
    function setenablec(data){if (data==1){$('#endelc').prop('checked', true);}};
    function setdelaya(data){$('#delaya').val(parseInt(data));};
    function setdelayb(data){$('#delayb').val(parseInt(data));};
    function setdelayc(data){$('#delayc').val(parseInt(data));};
    function setattenuation(data){$('#attenuation').val(parseFloat(data));};

    request_serial(setenablea, "CONFigure:DELay:Reset:ENable?");     
    request_serial(setenableb, "CONFigure:DELay:Data:ENable?"); 
    request_serial(setenablec, "CONFigure:DELay:Clock:ENable?"); 
    request_serial(setdelaya, "CONFigure:DELay:Reset:VALue?"); 
    request_serial(setdelayb, "CONFigure:DELay:Data:VALue?"); 
    request_serial(setdelayc, "CONFigure:DELay:Clock:VALue?"); 
    request_serial(setattenuation, "CONFigure:RF:ATTenuation?"); 
  });	
  </script>

</head>
<body><div class="ui-tabs ui-widget ui-widget-content ui-corner-all ui-resizable">



    <div>
      	<form>
            <label><input type="checkbox" name="endela" id="endela" value="Enable"/> Enable Delay Reset </label><br>
            <label><input type="checkbox" name="endelb" id="endelb" value="Enable"/> Enable Delay Data </label><br>
            <label><input type="checkbox" name="endelc" id="endelc" value="Enable"/> Enable Delay Clock </label><br>
            <label><input type="text"   name="delaya" id="delaya" min="0" max="11242" value="0">  Delay Reset Value (ps) </label><br>
            <label><input type="text"   name="delayb" id="delayb" min="0" max="11242" value="0">  Delay Data Value (ps) </label><br>
            <label><input type="text"   name="delayc" id="delayc" min="0" max="11242" value="0">  Delay Clock Value (ps) </label><br>
            <label><input type="text"   name="attenuation" id="attenuation" min="0" max="31.75" value="0">  Attenuation (dB) </label><br>
        </form>
    </div>

<!--    <div id="mqueue">
            <form>
                <input id="mqueueform" name="mqueueform" type="submit" value="Write" /> 
                <input id="mqueuebox" name="mqueuebox" type="textarea" value="" /> 
                <div id="mqueuewindow" style="height:120px;width:900px;background-color:white;overflow:auto;"></div>
            <form>
        </div>
-->            
</div></body>
</html>