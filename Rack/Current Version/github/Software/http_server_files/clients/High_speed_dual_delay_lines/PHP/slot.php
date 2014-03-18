<!--
Easy-phi project: http://easy-phi.ch/
Developer : Raphael Houlmann, University of Geneva  2014
Licensed under GPL, for details report to: http://easy-phi.ch/index.php/about/licensing
Do NOT remove this notice
-->
<link rel="stylesheet" href="/jquery/css/ui-lightness/jquery-ui-1.10.3.custom.css" />
<script src="/jquery/js/jquery-1.9.1.js"></script>
<script src="/jquery/js/jquery-ui-1.10.3.custom.js"></script>
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
            request_serial(none, ("CONFigure:DELay:A:ENable 1")); 
        } else {
            request_serial(none, ("CONFigure:DELay:A:ENable 0")); 
        }        
    });     

  	$('#endelb').change(function() {
        if ($(this).is(':checked')) {
            request_serial(none, ("CONFigure:DELay:B:ENable 1")); 
        } else {
            request_serial(none, ("CONFigure:DELay:B:ENable 0")); 
        }        
    });     

    
	$('#delaya').spinner({min: 0, max: 5115, step: 1, increment: 'fast', stop: function(event, ui) {
        request_serial(none, ("CONFigure:DELay:A:VALue " + ($(this).val()))); 
        }
    });     
    
	$('#delayb').spinner({min: 0, max: 5115, step: 1, increment: 'fast', stop: function(event, ui) {
        request_serial(none, ("CONFigure:DELay:B:VALue " + ($(this).val()))); 
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
    function setdelaya(data){$('#delaya').val(parseInt(data));};
    function setdelayb(data){$('#delayb').val(parseInt(data));};

    request_serial(setenablea, "CONFigure:DELay:A:ENable?");     
    request_serial(setenableb, "CONFigure:DELay:B:ENable?"); 
    request_serial(setdelaya, "CONFigure:DELay:A:VALue?"); 
    request_serial(setdelayb, "CONFigure:DELay:B:VALue?"); 
  });	
  </script>

</head>
<body><div class="ui-tabs ui-widget ui-widget-content ui-corner-all ui-resizable">



    <div>
      	<form>
            <label><input type="checkbox" name="endela" id="endela" value="Enable"/> Enable Delay A </label><br>
            <label><input type="checkbox" name="endelb" id="endelb" value="Enable"/> Enable Delay B </label><br>
            <label><input type="text"   name="delaya" id="delaya" min="0" max="5115" value="0">  Delay A Value (ps) </label><br>
            <label><input type="text"   name="delayb" id="delayb" min="0" max="5115" value="0">  Delay B Value (ps) </label><br>
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