<!--
Easy-phi project: http://easy-phi.ch/
Developer : Raphael Houlmann, University of Geneva   2014
Licensed under GPL, for details report to: http://easy-phi.ch/index.php/about/licensing
Do NOT remove this notice
-->
<link rel="stylesheet" href="/jquery/css/ui-lightness/jquery-ui-1.10.3.custom.css" />
<script src="/jquery/js/jquery-1.9.1.js"></script>
<script src="/jquery/js/jquery-ui-1.10.3.custom.js"></script>

<html>
<head>
          
  <script lang="text/javascript">
  $(document).ready(function(){
    <?php print("var port ="); print ($_GET["s"]+5555); ?>          
  	$('#mqueueform').submit(function(e) {
          $.ajax({
            url: "/php/easy-phi.php?p=" + port + "&q=" + $('#mqueuebox').val(),              
            async: true,
            success : function(data){
                 $('#mqueuewindow').append(data + '<br>');
                 $('#mqueuewindow').scrollTop($('#mqueuewindow')[0].scrollHeight);
  
             }
          });     
  		e.stopPropagation();
  		e.preventDefault();
  	});
  });	
  </script>

</head>
<body>
    <div id="mqueue">
    	<form id="mqueueform">
            <input id="mqueuebox" name="mqueuebox" type="textarea" value="" /> 
    		<input type="submit" value="Send" />
        </form>
    	<div id="mqueuewindow" style="height:120px;width:500px;background-color:white;overflow:auto;"></div>
    </div>
            
</body>
</html>