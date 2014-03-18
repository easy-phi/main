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
  	$('#mux').change(function() {request_serial(none, ("CONFigure:MUX " + ($(this).val())));});     
  	$('#pll_in').change(function() {request_serial(none, ("CONFigure:PLLin " + ($(this).val())));});     
  	$('#freq').spinner({min: 10, max: 50000, step: 1, increment: 'fast', stop: function(event, ui) {
        request_serial(none, ("CONFigure:FREQuence " + (($(this).val())*1000))); 
        }
    });     
/// initialisation    
    function initout1(data){
        data = data.replace(/(\r\n|\n|\r)/gm,""); //remove CR and NL characters
        if      (data=="PLL1"){$('#out1').val('PLL1');}
        else if (data=="PLL2"){$('#out1').val('PLL2');}
        else if (data=="MUX"){$('#out1').val('MUX');}
    }
    function initout2(data){
        data = data.replace(/(\r\n|\n|\r)/gm,""); //remove CR and NL characters
        if      (data=="PLL1"){$('#out2').val('PLL1');}
        else if (data=="PLL2"){$('#out2').val('PLL2');}
        else if (data=="MUX"){$('#out2').val('MUX');}
    }
    function initout3(data){
        data = data.replace(/(\r\n|\n|\r)/gm,""); //remove CR and NL characters
        if      (data=="PLL1"){$('#out3').val('PLL1');}
        else if (data=="PLL2"){$('#out3').val('PLL2');}
        else if (data=="MUX"){$('#out3').val('MUX');}
    }
    function initout4(data){
        data = data.replace(/(\r\n|\n|\r)/gm,""); //remove CR and NL characters
        if      (data=="PLL1"){$('#out4').val('PLL1');}
        else if (data=="PLL2"){$('#out4').val('PLL2');}
        else if (data=="MUX"){$('#out4').val('MUX');}
    }

    function initmux(data){
        data = data.replace(/(\r\n|\n|\r)/gm,""); //remove CR and NL characters
        if      (data=="REF_CLK"){$('#mux').val('REF_CLK');}
    }
    function initpllin(data){
        data = data.replace(/(\r\n|\n|\r)/gm,""); //remove CR and NL characters
        if      (data=="REF_CLK"){$('#pll_in').val('REF_CLK');}
    }
    function setfreq(data){$('#freq').val(parseInt(data/1000));};
    
    request_serial(initout1, "CONFigure:OUT1?");     
    request_serial(initout2, "CONFigure:OUT2?");     
    request_serial(initout3, "CONFigure:OUT3?");     
    request_serial(initout4, "CONFigure:OUT4?");     
    request_serial(initmux, "CONFigure:MUX?");     
    request_serial(initpllin,"CONFigure:PLLin?"); 
    request_serial(setfreq, "CONFigure:FREQuence?"); 
  });	
  </script>

</head>
<body><div class="ui-tabs ui-widget ui-widget-content ui-corner-all ui-resizable">

    <div>
        <form>
      	    <label for="select_type">Set output 1: 
            <select name="out1" id="out1">
                <option value="IN">IN</option>
                <option value="PLL1">PLL1</option>
                <option value="PLL2">PLL2</option>
                <option value="MUX">MUX</option>
            </select></label><br>
      	    <label for="select_type">Set output 2:
            <select name="out2" id="out2">
                <option value="IN">IN</option>
                <option value="PLL1">PLL1</option>
                <option value="PLL2">PLL2</option>
                <option value="MUX">MUX</option>
            </select></label><br>
      	    <label for="select_type">Set output 3:
            <select name="out3" id="out3">
                <option value="IN">IN</option>
                <option value="PLL1">PLL1</option>
                <option value="PLL2">PLL2</option>
                <option value="MUX">MUX</option>
            </select></label><br>
      	    <label for="select_type">Set output 4:
            <select name="out4" id="out4">
                <option value="IN">IN</option>
                <option value="PLL1">PLL1</option>
                <option value="PLL2">PLL2</option>
                <option value="MUX">MUX</option>
            </select></label><br><br>
      	    <label for="select_type">Configure MUX:
            <select name="mux" id="mux">
                <option value="UC">UC</option>
                <option value="REF_CLK">REF_CLK</option>
            </select></label><br>
      	    <label for="select_type">Set PLL Input:
            <select name="pll_in" id="pll_in">
                <option value="IN">IN</option>
                <option value="REF_CLK">REF_CLK</option>
            </select></label><br><br>
            <label><input type="text"   name="freq" id="freq" min="100" max="50000" value="100">  Frequence of test output kHZ </label><br>
            
        </form>
    </div>
<!--    <div id="mqueue">
            <form>
                <input id="mqueueform" name="mqueueform" type="submit" value="Write" /> 
                <input id="mqueuebox" name="mqueuebox" type="textarea" value="" /> 
                <div id="mqueuewindow"></div>
            <form>
        </div>
-->            
</div></body>
</html>