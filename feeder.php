<?php
if ( empty ($_GET['nachricht']) == TRUE )
{
    echo '
    <br><b>escucos catfeeder</b><br>
    <form action="feeder.php" method="get">
 
    <p>Text:
    <input type="text" name="nachricht" size="25" maxlength="20" />
    </p>
 
    <input type="submit" value="feed cats!" />
    </p> 
    </form>
    ';
}
else
{
	$info = substr($_GET['nachricht'], 0, 20);
	$socket = fsockopen('udp://192.168.2.60:1234');
	fputs($socket, $info);
	echo"<br><b>escucos catfeeder</b><br>";
	echo "<br>remote adress: ";
	echo $_SERVER['REMOTE_ADDR'];
	echo "<br>message: ";
  echo $info;
  echo "<br><b>Thanks for feeding!</b><br><br>";

}
?>
