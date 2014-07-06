<?php

include("db.php");
$uname=$_POST["name"];
$pass=$_POST["pwd"];
$uid=$_POST["uid"];
$city=$_POST["city"];
$state=$_POST["state"];
$email=$_POST["email"];
$phone=$_POST["ph"];
$pan=$_POST["pan"];

$a=mysql_query("insert into login values('$uname','$pass','$uid','$city','$state','$email','$phone','$pan')");

$result=mysql_query("select * from login");
mysql_close($con);

	echo "<center>";
	echo "<h1>"."successfully registered"."</h1>"."<br>";
	echo "<a href='index.html'>"."back"."</a>"."<br>";
	echo "</center>";

?>