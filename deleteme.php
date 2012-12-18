while ($row = mysql_fetch_array($database)) {
  echo "<tr>";
	echo "<td>".$row['first_name']." ".$row['last_name']."</td>";

	for($i = 0; $i < 14; $i++) {
		echo "<td><input type='checkbox' name='week1_".$row['id']."'";
		if(($row['attendance'] >> $i) & 1){ echo " checked=\"checked\"";} 
		if(($team_row['attendance'] >> $i) & 1 == '1'){ echo" disabled";};
		echo " onclick='processAttendance(\"".$row['username']."\",".$i.",this.checked)'></td>";
	}

	echo "</tr>";
}
