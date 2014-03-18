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
<!--<link href="/css/flot.css" rel="stylesheet" type="text/css">
<script language="javascript" type="text/javascript" src="/flot/jquery.js"></script>
<script language="javascript" type="text/javascript" src="/flot/jquery.flot.js"></script>
-->
<html>
<head>
          
  <script>
  $(document).ready(function(){
    <?php print("var port ="); print ($_GET["s"]+5555); ?>          
    var counta = 0;
    var countb = 0;
    var f1 = [];
    var f2 = [];
	for (var i = 0; i < 4; i += 1) {f1.push(0);}
	for (var i = 0; i < 4; i += 1) {f2.push(0);}
    
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
    
  	$('#coupa').change(function() {request_serial(none, ("CONFigure:CHAnnel:A:COUpling " + ($(this).val())));});     
  	$('#coupb').change(function() {request_serial(none, ("CONFigure:CHAnnel:B:COUpling " + ($(this).val())));});     
    
  	$('#enoffsa').change(function() {
        if ($(this).is(':checked')) {
            request_serial(none, ("CONFigure:CHAnnel:A:OFFset:ENable 1")); 
        } else {
            request_serial(none, ("CONFigure:CHAnnel:A:OFFset:ENable 0")); 
        }        
    });     

  	$('#enoffsb').change(function() {
        if ($(this).is(':checked')) {
            request_serial(none, ("CONFigure:CHAnnel:B:OFFset:ENable 1")); 
        } else {
            request_serial(none, ("CONFigure:CHAnnel:B:OFFset:ENable 0")); 
        }        
    });     

	$('#offseta').spinner({min: -2500, max: 2500, step: 1, increment: 'fast', stop: function(event, ui) {
        request_serial(none, ("CONFigure:CHAnnel:A:OFFset:VALue " + ($(this).val()))); 
        }
    });     
    
	$('#offsetb').spinner({min: -2500, max: 2500, step: 1, increment: 'fast', stop: function(event, ui) {
        request_serial(none, ("CONFigure:CHAnnel:B:OFFset:VALue " + ($(this).val()))); 
        }
    });     
   
	$('#thlda').spinner({min: -5000, max: 5000, step: 1, increment: 'fast', stop: function(event, ui) {
        request_serial(none, ("CONFigure:CHAnnel:A:THReshold " + ($(this).val()))); 
        }
    });     

	$('#thldb').spinner({min: -5000, max: 5000, step: 1, increment: 'fast', stop: function(event, ui) {
        request_serial(none, ("CONFigure:CHAnnel:B:THReshold " + ($(this).val()))); 
        }
    });     

    function display_cnta(data){
        data = data.replace(/(\r\n|\n|\r)/gm,""); //remove CR and NL characters
        if (data.indexOf("No new val") == -1){
            counta = counta + parseInt(data);
            $('#totcnta').html("Count A = <b><big>" + counta + "</big></b>");
            f1.shift();
            f1.push(parseInt(data));
            f = f1[0] + f1[1] + f1[2] + f1[3] 
            $('#freqcnta').html("Frequency Counter A = <b><big>" + f + "</big></b>");
            request_serial(display_cnta, "CONFigure:COUnter:A:Read?");        
        }
    }

    function display_cntb(data){
        data = data.replace(/(\r\n|\n|\r)/gm,""); //remove CR and NL characters
        if (data.indexOf("No new val") == -1){
            countb = countb + parseInt(data);
            $('#totcntb').html("Count B = <b><big>" + countb + "</big></b>");
            f2.shift();
            f2.push(parseInt(data));
            f = f2[0] + f2[1] + f2[2] + f2[3] 
            $('#freqcntb').html("Frequency Counter B = <b><big>" + f + "</big></b>");
            request_serial(display_cntb, "CONFigure:COUnter:B:Read?");        
        }
    }

  	$('#rsta').button().click(function(e) {
        counta =0;
        $('#totcnta').html("Count A = <b><big> 0 </big></b>");
  		e.stopPropagation();
  		e.preventDefault();
  	});

  	$('#rstb').button().click(function(e) {
        countb =0;
        $('#totcntb').html("Count B = <b><big> 0 </big></b>");
  		e.stopPropagation();
  		e.preventDefault();
  	});
    
  	$('#mqueueform').button().click(function(e) {
        request_serial(write_to_box, $('#mqueuebox').val()); 
  		e.stopPropagation();
  		e.preventDefault();
  	});
    
/// initialisation    
    function initcoupa(data){
        data = data.replace(/(\r\n|\n|\r)/gm,""); //remove CR and NL characters
        if      (data=="DC"){$('#coupa').val('DC');}
    }
    function initcoupb(data){
        data = data.replace(/(\r\n|\n|\r)/gm,""); //remove CR and NL characters
        if      (data=="DC"){$('#coupb').val('DC');}
    }
    function setenoffsa(data){if (data==1){$('#enoffsa').prop('checked', true);}};
    function setenoffsb(data){if (data==1){$('#enoffsb').prop('checked', true);}};
    function setoffa(data){$('#offseta').val(parseInt(data));};
    function setoffb(data){$('#offsetb').val(parseInt(data));};
    function settholda(data){$('#thlda').val(parseInt(data));};
    function settholdb(data){$('#thldb').val(parseInt(data));};
    
    request_serial(initcoupa, "CONFigure:CHAnnel:A:COUpling?");     
    request_serial(initcoupb, "CONFigure:CHAnnel:B:COUpling?");     
    request_serial(setenoffsa, "CONFigure:CHAnnel:A:OFFset:ENable?");     
    
    request_serial(setenoffsb, "CONFigure:CHAnnel:B:OFFset:ENable?"); 
    
    request_serial(setoffa, "CONFigure:CHAnnel:A:OFFset:VALue?"); 
    request_serial(setoffb, "CONFigure:CHAnnel:B:OFFset:VALue?"); 
    request_serial(settholda, "CONFigure:CHAnnel:A:THReshold?"); 
    request_serial(settholdb, "CONFigure:CHAnnel:B:THReshold?"); 
    setInterval(function(){request_serial(display_cnta, "CONFigure:COUnter:A:Read?"); },200);
    setInterval(function(){request_serial(display_cntb, "CONFigure:COUnter:B:Read?"); },200);        
  });	
  </script>



  
</head>
<body><div class="ui-tabs ui-widget ui-widget-content ui-corner-all ui-resizable">

    <div>
      	<form>
      	    <label for="select_type">Coupling channel A: 
            <select name="coupa" id="coupa">
                <option value="AC">AC</option>
                <option value="DC">DC</option>
            </select></label><br>
      	    <label for="select_type">Coupling channel B:
            <select name="coupb" id="coupb">
                <option value="AC">AC</option>
                <option value="DC">DC</option>
            </select></label><br>
            <label><input type="checkbox" name="enoffsa" id="enoffsa" value="Enable"/> Enable Offset on Channel A </label><br>
            <label><input type="checkbox" name="enoffsb" id="enoffsb" value="Enable"/> Enable Offset on Channel B </label><br>
            <label><input type="text"   name="offseta" id="offseta">  [mV] Channel A Offset</label><br>
            <label><input type="text"   name="offseta" id="offsetb">  [mV] Channel B Offset</label><br>
            <label><input type="text"   name="thlda" id="thlda">  [mV] Channel A Threshold</label><br>
            <label><input type="text"   name="thldb" id="thldb">  [mV] Channel B Threshold</label><br><br>
            <span style="display: inline-block;width: 230px;" id="totcnta">Count A = <b><big>0</big></b></span><input id="rsta" type="submit" value="Reset"><br>
            <span style="display: inline-block;width: 230px;" id="totcntb">Count B = <b><big>0</big></b></span><input id="rstb" type="submit" value="Reset"><br><br>
    	    <span id="freqcnta">Frequency Counter A = <b><big>0</big></b></span><br>
    	    <span id="freqcntb">Frequency Counter B = <b><big>0</big></b></span><br>
            
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