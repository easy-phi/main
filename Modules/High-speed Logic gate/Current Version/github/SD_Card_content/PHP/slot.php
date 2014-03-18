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
    
    
  	$('#out1').change(function() {request_serial(none, ("CONFigure:OUT1 " + ($(this).val())));});     
  	$('#out2').change(function() {request_serial(none, ("CONFigure:OUT2 " + ($(this).val())));});     
  	$('#out3').change(function() {request_serial(none, ("CONFigure:OUT3 " + ($(this).val())));});     
  	$('#out4').change(function() {request_serial(none, ("CONFigure:OUT4 " + ($(this).val())));});     
/// initialisation    
    function initout1(data){
        data = data.replace(/(\r\n|\n|\r)/gm,""); //remove CR and NL characters
        if      (data=="AND"){$('#out1').val('AND');}
        else if (data=="IN1"){$('#out1').val('IN1');}
        else if (data=="IN2"){$('#out1').val('IN2');}
    }
    function initout2(data){
        data = data.replace(/(\r\n|\n|\r)/gm,""); //remove CR and NL characters
        if      (data=="AND"){$('#out2').val('AND');}
        else if (data=="IN1"){$('#out2').val('IN1');}
        else if (data=="IN2"){$('#out2').val('IN2');}
    }
    function initout3(data){
        data = data.replace(/(\r\n|\n|\r)/gm,""); //remove CR and NL characters
        if      (data=="AND"){$('#out3').val('AND');}
        else if (data=="IN1"){$('#out3').val('IN1');}
        else if (data=="IN2"){$('#out3').val('IN2');}
    }
    function initout4(data){
        data = data.replace(/(\r\n|\n|\r)/gm,""); //remove CR and NL characters
        if      (data=="AND"){$('#out4').val('AND');}
        else if (data=="IN1"){$('#out4').val('IN1');}
        else if (data=="IN2"){$('#out4').val('IN2');}
    }
    
    request_serial(initout1, "CONFigure:OUT1?");     
    request_serial(initout2, "CONFigure:OUT2?");     
    request_serial(initout3, "CONFigure:OUT3?");     
    request_serial(initout4, "CONFigure:OUT4?");     
  });	
  </script>

</head>
<body><div class="ui-tabs ui-widget ui-widget-content ui-corner-all ui-resizable">

    <div>
        <form>
      	    <label for="select_type">Set output 1: 
            <select name="out1" id="out1">
                <option value="OR">OR</option>
                <option value="AND">AND</option>
                <option value="IN1">IN1</option>
                <option value="IN2">IN2</option>
            </select></label><br>
      	    <label for="select_type">Set output 2:
            <select name="out2" id="out2">
                <option value="OR">OR</option>
                <option value="AND">AND</option>
                <option value="IN1">IN1</option>
                <option value="IN2">IN2</option>
            </select></label><br>
      	    <label for="select_type">Set output 3:
            <select name="out3" id="out3">
                <option value="OR">OR</option>
                <option value="AND">AND</option>
                <option value="IN1">IN1</option>
                <option value="IN2">IN2</option>
            </select></label><br>
      	    <label for="select_type">Set output 4:
            <select name="out4" id="out4">
                <option value="OR">OR</option>
                <option value="AND">AND</option>
                <option value="IN1">IN1</option>
                <option value="IN2">IN2</option>
            </select></label><br>
            
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