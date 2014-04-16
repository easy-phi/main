
///////Funtions to test if the string contains float or integer

function isFloat(n) {
  return !isNaN(parseFloat(n)) && isFinite(n);
}

function isInteger(n) {
  return !isNaN(parseInt(n)) && isFinite(n);
}

////USAGE EXAMPLE
////var n = "-5.6E4"

////if (isFloat(n)) {
////    div.textContent = "n is contains a float value: " + parseFloat(n) ;
////} else {
////    div.textContent = "not a float" ;
////}
//// 
////
////if (isInteger(n)) {
////    div2.textContent = "n is contains a integer value"  + parseInt(n) ;
////} else {
////    div2.textContent = "not a int"  ;
////}

