Geriatric Fast Facts
===========================


Core Update Notes
===========================

A fix in the core files is necessary when upgrading.  This hackish fix 
involves updating the database file at the path:

/includes/database/database.inc
The line can be found at around line ~2170.

In order to implement this change, surround the following line with a try/catch block:
$return = parent::execute($args);

like such:
try{
	$return = parent::execute($args);
}catch(Exception $e){
	//Do nothing
}
