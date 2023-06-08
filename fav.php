<?php
if (isset($_COOKIE['favorite_facts'])) {
	$favlist = preg_split('/;/', $_COOKIE['favorite_facts'], -1, PREG_SPLIT_NO_EMPTY);
	print count($favlist);
} else {
	print "0";
}
?>