<?php
$link = mysqli_connect('localhost','database user','database password');
$database = mysqli_select_db($link,'database name');

$user = $_GET['username'];

$sql = "SELECT * FROM mybb_users WHERE username = '". mysqli_real_escape_string($link,$user) ."'" ;
$result = $link->query($sql);

if ($result->num_rows > 0) {

	while($row = $result->fetch_assoc()) {
		echo $row['usergroup'];			
	}
} 
?>