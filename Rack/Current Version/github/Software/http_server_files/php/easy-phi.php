<?php
/*
Easy-phi project: http://easy-phi.ch/
Developer : Raphael Houlmann, University of Geneva   2014
Licensed under GPL, for details report to: http://easy-phi.ch/index.php/about/licensing
Do NOT remove this notice
*/

$message=$_REQUEST["q"];
$port=$_REQUEST["p"];

$context = new ZMQContext();
$soc = new ZMQSocket($context, ZMQ::SOCKET_REQ);
$soc->connect('tcp://localhost:'.$port);
$soc->send($message);
$answer = $soc->recv();
echo $answer;
exit();
?>
