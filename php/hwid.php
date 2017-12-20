<?php
$link = mysqli_connect('localhost','database user','database password');
$database = mysqli_select_db($link,'database name');

$user = $_GET['username'];
$hwid = $_GET['hwid'];

$sql = "SELECT * FROM mybb_users WHERE username = '". mysqli_real_escape_string($link,$user) ."'" ;
$result = $link->query($sql);

if(strlen($hwid) < 1)
{
	echo "hwiderror";
}
else
{
	if ($result->num_rows > 0) {
		while($row = $result->fetch_assoc()) {
			if (strlen($row['hwid']) > 1)
			{
				if ($hwid != $row['hwid'])
				{
					echo "hwiderror";
				}
				else
				{
				}
			}
			else
			{
				$sql = "UPDATE mybb_users SET hwid='$hwid' WHERE username='$user'";
				if(mysqli_query($link, $sql))
				{
					echo $row['hwid'];
				}
				else
				{
					echo "ERROR: Could not able to execute $sql. " . mysqli_error($link);
				}
			}
		}
	}
}



?>