<?php
include("inc_dbconfig.php");
define('LINE_API',"https://notify-api.line.me/api/notify");

//$token = "Ot1gZJof9119lYbLC7VRQb7eafXVmCqvAbr9r2p6aoA"; //hanami
//$token = "lczHoxFWTwFC3vjJgNEUy1nPRGadvKmQAuLuvxRWQTr"; //gps.scsthai.com
// Farmsook : rPhZTSxTWCckYGtmt7tk5qemfZMvuPeWSC0VbPDYFkq
$token = "rPhZTSxTWCckYGtmt7tk5qemfZMvuPeWSC0VbPDYFkq";
//$token = "lczHoxFWTwFC3vjJgNEUy1nPRGadvKmQAuLuvxRWQTr";
function notify_message($message,$token){
 $queryData = array('message' => $message);
 $queryData = http_build_query($queryData,'','&');
 $headerOptions = array( 
         'http'=>array(
            'method'=>'POST',
            'header'=> "Content-Type: application/x-www-form-urlencoded\r\n"
                      ."Authorization: Bearer ".$token."\r\n"
                      ."Content-Length: ".strlen($queryData)."\r\n",
            'content' => $queryData
         ),
 );
 $context = stream_context_create($headerOptions);
 $result = file_get_contents(LINE_API,FALSE,$context);
 $res = json_decode($result);
 return $res;
}
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
        <title>Smart Connect Solutions Co.,Ltd.</title>
        <link href="~/Images/SCS_Icon.ico?v=1.1" rel="shortcut icon" type="image/x-icon" />
        <link href="style.css" rel="stylesheet" type="text/css" />
    </head>
<body>
<?php
function get_TrkFlg($mobno)
{
	include("inc_dbconfig.php");
	$sql = "SELECT vbrcfg
			FROM  `gpstrack` 
			WHERE mobno = '".$mobno."'";
	$res = $db->Execute($sql);
	return	$res->fields[0];
}
function get_VActive($mobno)
{
	include("inc_dbconfig.php");
	$sql = "SELECT vactiv
			FROM  `gpstrack` 
			WHERE mobno = '".$mobno."'";
	$res = $db->Execute($sql);
	echo $sql."<br/>";
	return	$res->fields[0];
}
function get_token($mobno)
{
	include("inc_dbconfig.php");
	$sql = "SELECT token
			FROM  `gpstrack` 
			WHERE mobno = '".$mobno."'";
	$res = $db->Execute($sql);
	echo $sql."<br/>";
	return	$res->fields[0];
}
	if ($_GET[m] != "")
	{
		//http://localhost:8888/API/api.php?mob=0959530130&msg=10.12|11.13|12.14|13.15|14.16|15.17|20.19|1499921386
		//http://localhost:8888/api/?mob=0959530130,100.4567,144.1235
		//http://gps.scsthai.com/api.php?mob=0959530130&msg=10.12|11.13|12.14|13.15|14.16|15.17|20.19|1499921386
		//http://gps.scsthai.com/pir.php?m=0959530130
		if (strlen($_GET[m]) == "") exit();
		$myStr = $_GET[m];
		$strMsg = "ตรวจพบความเคลื่อนไหว ".date("Y-m-d h:i:s");
		notify_message($strMsg,$token);	
		
	}
?>
</body>