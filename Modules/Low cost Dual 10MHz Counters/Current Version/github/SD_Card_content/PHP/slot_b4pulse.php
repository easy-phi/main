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

<!--<link href="/css/flot.css" rel="stylesheet" type="text/css">
<script language="javascript" type="text/javascript" src="/flot/jquery.js"></script>
<script language="javascript" type="text/javascript" src="/flot/jquery.flot.js"></script>
-->
<html>
<head>
          
  <script>
  $(document).ready(function(){
    <?php print("var port ="); print ($_GET["s"]+5555); ?>          
    //var d1 = [];
    //var d2 = [];
	//for (var i = 0; i < 10; i += 1) {d1.push([i, 0]);}
	//for (var i = 0; i < 10; i += 1) {d2.push([i, 0]);}

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
    
    
  	$('#encnta').change(function() {
        if ($(this).is(':checked')) {
            request_serial(none, ("CONFigure:COUnter:A:ENable 1")); 
        } else {
            request_serial(none, ("CONFigure:COUnter:A:ENable 0")); 
        }        
    });     

  	$('#encntb').change(function() {
        if ($(this).is(':checked')) {
            request_serial(none, ("CONFigure:COUnter:B:ENable 1")); 
        } else {
            request_serial(none, ("CONFigure:COUnter:B:ENable 0")); 
        }        
    });     

////	$('#cntaintt').spinner({min: 1, max: 240, step: 1, increment: 'fast', stop: function(event, ui) {
////        request_serial(none, ("CONFigure:COUnter:A:INTegrationtime " + ($(this).val()))); 
////        }
////    });     
////    
////	$('#cntbintt').spinner({min: 1, max: 240, step: 1, increment: 'fast', stop: function(event, ui) {
////        request_serial(none, ("CONFigure:COUnter:B:INTegrationtime " + ($(this).val()))); 
////        }
////    });     

  	$('#ttl').change(function() {
        if ($(this).is(':checked')) {
            request_serial(none, ("CONFigure:TTLinputenable 1")); 
        } else {
            request_serial(none, ("CONFigure:TTLinputenable 0")); 
        }        
    });     

  	$('#freq').spinner({min: 10, max: 50000, step: 1, increment: 'fast', stop: function(event, ui) {
        var n = ($(this).val());
        if (isInteger(n)) {
            request_serial(none, ("CONFigure:FREQuence " + (parseInt(n))*1000)); 
        }    
        }    
    });     
    

    function display_cnta(data){
        data = data.replace(/(\r\n|\n|\r)/gm,""); //remove CR and NL characters
        if (data.indexOf("No new val") == -1){
            //d1[0][1] = d1[1][1];
            //d1[1][1] = d1[2][1];
            //d1[2][1] = d1[3][1];
            //d1[3][1] = d1[4][1];
            //d1[4][1] = d1[5][1];
            //d1[5][1] = d1[6][1];
            //d1[6][1] = d1[7][1];
            //d1[7][1] = d1[8][1];
            //d1[8][1] = d1[9][1];
            //d1[9][1] = data;
            //$.plot("#cnta", [d1]);
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
            //d2[0][1] = d2[1][1];
            //d2[1][1] = d2[2][1];
            //d2[2][1] = d2[3][1];
            //d2[3][1] = d2[4][1];
            //d2[4][1] = d2[5][1];
            //d2[5][1] = d2[6][1];
            //d2[6][1] = d2[7][1];
            //d2[7][1] = d2[8][1];
            //d2[8][1] = d2[9][1];
            //d2[9][1] = data;
	        //$.plot("#cntb", [d2]);
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
    function setenablea(data){if (data==1){$('#encnta').prop('checked', true);}};
    function setenableb(data){if (data==1){$('#encntb').prop('checked', true);}};
    function setcntaintt(data){$('#cntaintt').val(parseInt(data));};
    function setcntbintt(data){$('#cntbintt').val(parseInt(data));};
    function setenablettl(data){if (data==1){$('#ttl').prop('checked', true);}};
    function setfreq(data){$('#freq').val(parseInt(data/1000));};
    
    request_serial(setenablea, "CONFigure:COUnter:A:ENable?");     
    request_serial(setenableb, "CONFigure:COUnter:B:ENable?"); 
    //request_serial(setcntaintt, "CONFigure:COUnter:A:INTegrationtime?"); 
    //request_serial(setcntbintt, "CONFigure:COUnter:B:INTegrationtime?"); 
    request_serial(setenablettl, "CONFigure:TTLinputenable?"); 
    request_serial(setfreq, "CONFigure:FREQuence?"); 
    setInterval(function(){request_serial(display_cnta, "CONFigure:COUnter:A:Read?"); },200);
    setInterval(function(){request_serial(display_cntb, "CONFigure:COUnter:B:Read?"); },200);    
	
    
  });	
  </script>



  
</head>
<body><div class="ui-tabs ui-widget ui-widget-content ui-corner-all ui-resizable">

    <div>
      	<form>
            <label><input type="checkbox" name="encnta" id="encnta" value="Enable"/> Enable counter A </label><br>
            <label><input type="checkbox" name="encntb" id="encntb" value="Enable"/> Enable counter B </label><br><br><br>

<!--
            <label><input type="text"   name="cntaintt" id="cntaintt" min="1" max="240" value="0">  Counter A integration time </label><br>
            <label><input type="text"   name="cntbintt" id="cntbintt" min="1" max="240" value="0">  Counter B integration time </label><br><br><br>
-->
            <label><input type="checkbox" name="ttl" id="ttl" value="Enable"/> Set QMA to the TTL output </label><br>
            <label><input type="text"   name="freq" id="freq" min="100" max="50000" value="100">  Frequence of test output kHZ </label><br><br><br>
            <span style="display: inline-block;width: 230px;" id="totcnta">Count A = <b><big>0</big></b></span><input id="rsta" type="submit" value="Reset"><br>
            <span style="display: inline-block;width: 230px;" id="totcntb">Count B = <b><big>0</big></b></span><input id="rstb" type="submit" value="Reset"><br><br><br>
    	    <span id="freqcnta">Frequency Counter A = <b><big>0</big></b></span><br>
    	    <span id="freqcntb">Frequency Counter B = <b><big>0</big></b></span><br>
            
        </form>
    </div>
<!--	<div class="demo-container">
		<div id="cnta" class="demo-placeholder"></div>
	</div>
	<div class="demo-container">
		<div id="cntb" class="demo-placeholder"></div>
	</div>
-->
    
    
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