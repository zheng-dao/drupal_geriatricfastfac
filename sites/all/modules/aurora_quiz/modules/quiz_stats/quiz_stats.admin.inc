<?php

/**
 * @file
 * Administration file for Quiz stats Module
 *
 */

/**
 * Page for selecting between several quiz revisions
 *
 * @param $nid
 *  Quiz node id
 * @return
 *  Themed html
 */
function quiz_stats_revision_selector_page($quiz) {
  $res = db_query('SELECT vid FROM {node_revision} WHERE nid = :nid ORDER BY vid', array(':nid' => $quiz->nid));
  $count = 0; //uses this variable to slightly increase performance
  $vids = array();
  while ($res_o = $res->fetch()) {
    $vids[] = $res_o->vid;
    $count++;
  }
  if ($count < 1) {
    return t('Something went wrong. Please try again');
  }
  elseif ($count == 1) {
    // If there is only one revision we jump directly to that revision
    drupal_goto("node/{$quiz->nid}/quiz/statistics/{$vids[0]}");
  }
  else {
    $content = array();
    $content['explanation'] = t('There are !num revisions of this quiz that have been taken.
    Different revisions may have different scoring, difficulity and other differences affecting its statistics.
    Because of this you have to choose the revision you want to see statistics from.', array('!num' => $count));
    $content['links'] = array();
    foreach ($vids as $key => $value) {
      $content['links'][] = 'node/' . $quiz->nid . '/quiz/statistics/' . $value;
    }
    return theme('quiz_stats_revision_selector', array('content' => $content));
  }
}

/**
 * Page for selecting between several quiz revisions all answers
 *
 * @param $nid
 *  Quiz node id
 * @return
 *  Themed html
 */
function quiz_stats_revision_selector_all_enrollment_page($quiz) {
  $res = db_query('SELECT vid FROM {node_revision} WHERE nid = :nid ORDER BY vid', array(':nid' => $quiz->nid));
  $count = 0; //uses this variable to slightly increase performance
  $vids = array();
  while ($res_o = $res->fetch()) {
    $vids[] = $res_o->vid;
    $count++;
  }

  if ($count < 1) {
    return t('Something went wrong. Please try again');
  }
  elseif ($count == 1) {
    // If there is only one revision we jump directly to that revision
    #print "count is 1";
	drupal_goto("node/{$quiz->nid}/quiz/all-enrollment/{$vids[0]}");
  }
  else {
    $content = array();
    $content['explanation'] = t('There are !num revisions of this quiz that have been taken.
    Different revisions may have different scoring, difficulity and other differences affecting its statistics.
    Because of this you have to choose the revision you want to see statistics from.', array('!num' => $count));
    $content['links'] = array();
    foreach ($vids as $key => $value) {
      $content['links'][] = 'node/' . $quiz->nid . '/quiz/all-enrollment/' . $value;
    }
	#print_r($content);
    return theme('quiz_stats_revision_selector', array('content' => $content));
  }
}


/**
 * Page for selecting between several quiz revisions all answers
 *
 * @param $nid
 *  Quiz node id
 * @return
 *  Themed html
 */
function quiz_stats_revision_selector_all_answers_by_quiz($quiz) {
  $res = db_query('SELECT vid FROM {node_revision} WHERE nid = :nid ORDER BY vid', array(':nid' => $quiz->nid));
  $count = 0; //uses this variable to slightly increase performance
  $vids = array();
  while ($res_o = $res->fetch()) {
    $vids[] = $res_o->vid;
    $count++;
  }

  if ($count < 1) {
    return t('Something went wrong. Please try again');
  }
  elseif ($count == 1) {
    // If there is only one revision we jump directly to that revision
    #print "count is 1";
	drupal_goto("node/{$quiz->nid}/quiz/all-answers/{$vids[0]}");
  }
  else {
    $content = array();
    $content['explanation'] = t('There are !num revisions of this quiz that have been taken.
    Different revisions may have different scoring, difficulity and other differences affecting its statistics.
    Because of this you have to choose the revision you want to see statistics from.', array('!num' => $count));
    $content['links'] = array();
    foreach ($vids as $key => $value) {
      $content['links'][] = 'node/' . $quiz->nid . '/quiz/all-answers/' . $value;
    }
	#print_r($content);
    return theme('quiz_stats_revision_selector', array('content' => $content));
  }
}

/**
 * Page for selecting between several quiz revisions all answers
 *
 * @param $nid
 *  Quiz node id
 * @return
 *  Themed html
 */
function quiz_stats_revision_selector_aggregate_answers_by_quiz($quiz) {
  $res = db_query('SELECT vid FROM {node_revision} WHERE nid = :nid ORDER BY vid', array(':nid' => $quiz->nid));
  $count = 0; //uses this variable to slightly increase performance
  $vids = array();
  while ($res_o = $res->fetch()) {
    $vids[] = $res_o->vid;
    $count++;
  }

  if ($count < 1) {
    return t('Something went wrong. Please try again');
  }
  elseif ($count == 1) {
    // If there is only one revision we jump directly to that revision
    #print "count is 1";
	drupal_goto("node/{$quiz->nid}/quiz/aggregate-answers/{$vids[0]}");
  }
  else {
    $content = array();
    $content['explanation'] = t('There are !num revisions of this quiz that have been taken.
    Different revisions may have different scoring, difficulity and other differences affecting its statistics.
    Because of this you have to choose the revision you want to see statistics from.', array('!num' => $count));
    $content['links'] = array();
    foreach ($vids as $key => $value) {
      $content['links'][] = 'node/' . $quiz->nid . '/quiz/aggregate-answers/' . $value;
    }
	#print_r($content);
    return theme('quiz_stats_revision_selector', array('content' => $content));
  }
}



/**
 * Returns statistics for all available quizzes
 *
 * @param $uid
 *   User id
 * @return
 *   HTML table
 */
function quiz_stats_get_all_enrollment($uid = 0) {
  $results = _quiz_get_quizzes($uid);
  return empty($results) ? t('No Enrollments found.') : theme('quiz_stats_get_all_enrollment', array('results' => $results));
}


/**
 * Returns statistics for all available quizzes
 *
 * @param $uid
 *   User id
 * @return
 *   HTML table
 */
function quiz_stats_get_all_answers_by_quiz($uid = 0) {
  $results = _quiz_get_quizzes($uid);
  return empty($results) ? t('No Quizzes found.') : theme('quiz_stats_get_all_answers_by_quiz', array('results' => $results));
}


/**
 * Returns answers for all available quizzes
 *
 * @param $uid
 *   User id
 * @return
 *   HTML table
 */
function quiz_stats_get_basic_stats($uid = 0) {
  $results = _quiz_get_quizzes($uid);
  return empty($results) ? t('No Statistics found.') : theme('quiz_stats_get_basic_stats', array('results' => $results));
}


/**
 * Get stats for a single quiz. Maybe also for a single user.
 *
 * @param $vid
 *   quiz revision id
 *
 * @param $uid
 *   User id
 *
 * @return
 *   HTML page with charts/graphs
 */
function quiz_stats_get_adv_stats($vid, $uid = 0) {
  $charts = array();
  drupal_add_css(drupal_get_path('module', 'quiz_stats') . '/quiz_stats.css', array('type' => 'file', 'weight' => CSS_THEME));
  $charts['takeup'] = _get_date_vs_takeup_count_chart($vid, $uid);
  // line chart/graph showing quiz takeup date along x-axis and count along y-axis
  $charts['status'] = _get_quiz_status_chart($vid, $uid);
  // 3D pie chart showing percentage of pass, fail, incomplete quiz status
  $charts['top_scorers'] = _get_quiz_top_scorers_chart($vid, $uid);
  // Bar chart displaying top scorers
  $charts['grade_range'] = _get_quiz_grade_range_chart($vid, $uid);
  return theme('quiz_stats_charts', array('charts' => $charts));
}


/**
 * Get stats for a single quiz. Maybe also for a single user.
 *
 * @param $vid
 *   quiz revision id
 *
 * @param $uid
 *   User id
 *
 * @return
 *   HTML page with charts/graphs
 */
function quiz_stats_get_all_quiz_enrollment($nid, $vid) {
	$output = "";
	#drupal_load('module', 'taxonomy');
  print "i'm in quiz_stats_get_all_quiz_enrollment";
  #print $vid;
  #print_r($nid->nid);
  #print_r($vid);
  $quiztitle = _quiz_title($nid->nid);
  $output .= $quiztitle;
  $output .= _quiz_export_link();

   $results = _quiz_get_enrollment($vid, $nid->nid);
  #$results = $variables['results'];
  #print_r($results);
  if (empty($results)) {
    return '<p>' . t('No demographic information was found for this quiz.') . '</p>';
  }
 # print "but why";
  $header = $rows = array();
  $header = array(
    'part_no' => t('Participant #'),
    #'title' => t('Name'),
	'email' => t('E-mail'),
	'profession' => t('Profession'),
	'organization' => t('Organization'),
    'additionalrecipients' => t('Additional Recipients'),

	'completed' => t('Date Submitted'),
  );

  foreach ($results as $result) {
    if (user_access('access author stats')) {
		if (trim($result['email']) == "") {
			$email = "Anonymous";
			$thisprof = "Anonymous";
			$addl = "Anonymous";
			$inst = "Anonymous";
		} else {
			if ($result['profession'] == 0) {
				$thisprof = "N/A";
			} else {
			$prof = taxonomy_term_load(trim($result['profession']));
			$thisprof = $prof->name;
			#print_r($prof);
			}
			$email = $result['email'];

			$addl = $result['additional_email'];
			$inst = $result['institution'];
		}
		if ($result['time_end'] == "") {
			$comp = "Not Completed";
		} else {
			$comp = date("F j, Y - g:i a", $result['time_end']);
		}

	#print_r($prof);
	$row['part_no'] = $result['result_id'];
    #$row['title'] = "participant Name";

	$row['email'] = $email;
	$row['profession'] = $thisprof;
    $row['organization'] = $inst;
    $row['additionalrecipients'] = $addl;
	$row['completed'] = $comp;
    }
    $rows[] = $row;
  }
  module_load_include('inc', 'quiz', 'quiz.pages');


 if ($_GET['export']) {
  $csvarray = array('header' => header, 'rows' => $rows);
  $csvarray = $rows;
  #print_r($csvarray);
  convert_to_csv($csvarray, $quiztitle.'.csv', ',');
  } else {
  $output .= theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'tablesorter')));
  return $output;
  }
}

/**
	* Get a list of all enrollees with demographic info by quiz/revision.
	*
	* @param $vid
	*
	* @return
	*   A list of enrollees .
	*/
function _quiz_get_enrollment($vid, $nid) {
	$results = array();

	/**/
	$query = db_select('quiz_demographics_user_answers', 'da')
		->fields('da', array('result_id', 'email', 'profession', 'institution', 'additional_email'))
		->fields('q', array('time_end', 'nid', 'time_start', 'score', 'vid'));
	$query->leftJoin('quiz_node_results', 'q', 'q.result_id = da.result_id');

	$query->condition('q.vid', $vid);
	$query->condition('q.nid', $nid);
	$quizzes = $query->execute();

			/* $query  = "SELECT *
						FROM {quiz_node_results} ua
						JOIN {quiz_demographics_user_answers} uad
						ON uad.result_id = ua.result_id
						AND ua.nid = :nid
						AND ua.vid = :vid";

						$arguments = array(
							':nid' => $nid,
							':vid' => $vid
						); */

				/*$query  = "SELECT result_id
						FROM quiz_node_results
						WHERE nid = :nid
						AND vid = :vid";

						$arguments = array(
							':nid' => $nid,
							':vid' => $vid
						);

						$res = db_query($query, $arguments);
						$user_answers = $res->fetchAll();
						#print_r($user_answers);
						#print count($user_answers);
						$moo = "";
						foreach($user_answers as $user_answer) {
							#print_r($user_answer->result_id);
							print $user_answer->result_id .",";
							#$moo .= $moo . $user_answer->result_id;
						}
						#print $moo;

						foreach($user_answers as $user_answer){
							$answercorrect[] = _get_quiz_answer_text_mca($user_answer->answer_id);
							#$answercorrect[] = $user_answer->answer_id;
						}
						if (sizeof($user_answers) < 1) {
							$answercorrect[] = "0";
						}*/



	foreach ($quizzes as $quiz) {
		#print $quiz->result_id."<br>";
		$results[$quiz->result_id] = (array) $quiz;
	}

	return $results;
}

function _quiz_get_enrollment_by_result($rid) {
	$results = array();
	$query = db_select('quiz_demographics_user_answers', 'da')
		->fields('da', array('result_id', 'email', 'profession', 'institution', 'additional_email'));;
	$query->condition('da.result_id', $rid);
	$demos = $query->execute();

	foreach ($demos as $demo) {
		#print_r($demo);
		if (!empty($demo)) {
		#$demo->rid = $rid;
		$results[] = $demo;
		}
	}
	#print_r($results);
	return $results;
}

function _quiz_get_everybody() {
	$results = array();
	$query = db_select('quiz_demographics_user_answers', 'da')
		->fields('da', array('result_id', 'email', 'profession', 'institution', 'additional_email'))
		->fields('q', array('time_end', 'nid', 'time_start', 'score', 'vid'));
	$query->leftJoin('quiz_node_results', 'q', 'q.result_id = da.result_id');

	#$query->condition('q.vid', $vid);
	#$query->condition('q.nid', $nid);
	$quizzes = $query->execute();

	foreach ($quizzes as $quiz) {
		$results[$quiz->result_id] = (array) $quiz;
	}

	return $results;
}

/* overall report */

function quiz_stats_get_overall_report() {
$output = "";
 $headeroverall = $overall = array();
   $headeroverall = array(
  	'totals' => t('Quiz'),
    'part_no' => t('Enrolled'),
	'compl' => t('Completed'),
	'perccorr' => t('Overall % Correct'),
  );

	$allquizrevisions = db_query('SELECT nid,vid FROM {node} WHERE type = :nodetype ORDER BY nid', array(':nodetype' => 'quiz'));
	$enrolled = "";
	$completed = "";
	$totenroll = "";
	$totcomplete = "";
	$temppercent = "";
	$percentage = array();


	   while ($res_o = $allquizrevisions->fetch()) {
		   $quiz = node_load($res_o->nid);
		   $quiztitle = $quiz->title ;
			#print_r($quiz->field_gff_number['und'][0]['safe_value']);
			 $resultstotals[] = _quiz_get_total_enrollees($res_o->vid, $res_o->nid);
			 $questions = _quiz_get_correct_answer_info($res_o->vid, $res_o->nid);
			 foreach ($resultstotals as $resultstots) {
				 $tempcorrect = "";
				 $tempcomplete = "";
				 $temppercent = "";
				 $temptotalanswers = "";
				#print_r($resultstots);
				#$enrolled = $enrolled + $resultstots->totalenrolled;
				$enrolled = $resultstots->totalenrolled;
				$completed = $resultstots->totalcompleted;

				$overall[$res_o->nid]['quiz'] = $quiz->title .' GFF - '. $quiz->field_fast_fact_number['und'][0]['value'];
				$overall[$res_o->nid]['te'] = $resultstots->totalenrolled;
				$overall[$res_o->nid]['tc'] = $resultstots->totalcompleted;

				foreach ($questions as $qkey => $qresult) {
					#print $qresult['totalcorrect'] ."-";
					$tempcorrect = $tempcorrect + $qresult['totalcorrect'];
					$temptotalanswers = $temptotalanswers + $qresult['totalanswers'];
					#print $tempcorrect ." - ". $temptotalanswers ."<br>";
				}

				$tempenroll = $enrolled;
				$tempcomplete = $completed;
				#$temppercent = round($resultstots->averagescore, 2);
				$temppercent = round($tempcorrect/$temptotalanswers, 2) * 100 .'%';
				$overall[$res_o->nid]['avg'] = round($tempcorrect/$temptotalanswers, 2) * 100 .'%';
			  }

			  $percentage[] = $temppercent;

		   $totenroll = $totenroll + $tempenroll;
		   $totcomplete = $totcomplete + $tempcomplete;
		   $totpercent = $temppercent + $temppercent;

		   $displaytotalenroll = $totenroll;
		   $displaytotalcomplete = $totcomplete;

		}
		#print_r($percentage);
		#print array_sum($percentage) ."<br>";
		#print count($percentage);
		$average_of_foo = array_sum($percentage) / count($percentage);

	$overall['totals']['quiz'] = t('<b>Total:</b>');
	$overall['totals']['te'] = $displaytotalenroll;
	$overall['totals']['tc'] = $totcomplete;
	$overall['totals']['avg'] =  round($average_of_foo, 2).'%';

	$quiztitle = "Overall Report";
	$output .= $quiztitle;
	$output .= _quiz_export_link();

	if ($_GET['export']) {
	$csvarray = array('header' => header, 'rows' => $overall);
	$csvarray = $overall;
	#print_r($csvarray);
	convert_to_csv($csvarray, $quiztitle.'.csv', ',');
	} else {
	$output .= theme('table', array('header' => $headeroverall, 'rows' => $overall, 'attributes' => array('id' => 'tablesorter')));
	return $output;
	}

	#$output .= theme('table', array('header' => $headeroverall, 'rows' => $overall, 'attributes' => array('id' => 'tablesorter')));
	#return $output;
}


/* all enrollment  report */

function quiz_stats_get_all_enrollment_report($nid,$vid) {
$output = "";
#print "bah";
#print $nid;
#print $vid;

 $results = _quiz_get_everybody();

  #$results = $variables['results'];
  #print_r($results);
  if (empty($results)) {
    return '<p>' . t('No demographic information was found for this quiz.') . '</p>';
  }
 # print "but why";
  $header = $rows = array();
  $header = array(
  	'quiz' => t('Quiz'),
    'part_no' => t('Participant #'),
	'email' => t('E-mail'),
	'profession' => t('Profession'),
	'organization' => t('Organization'),
    'additionalrecipients' => t('Additional Recipients'),

	'completed' => t('Date Submitted'),
  );

  foreach ($results as $result) {
    if (user_access('access author stats')) {
		if (trim($result['email']) == "") {
			$email = "Anonymous";
			$thisprof = "Anonymous";
			$addl = "Anonymous";
			$inst = "Anonymous";
		} else {
			$prof = taxonomy_term_load(trim($result['profession']));
			$email = $result['email'];

			$thisprof = $prof->name;
			$addl = $result['additional_email'];
			$inst = $result['institution'];
		}
		if ($result['time_end'] == "") {
			$comp = "Not Completed";
		} else {
			$comp = date("F j, Y - g:i a", $result['time_end']);
		}

	#print_r($prof);
	$quiztitle = _quiz_title($result['nid']);
	$row['quiz'] = $quiztitle;
	$row['part_no'] = $result['result_id'];
    $row['email'] = $email;
	$row['profession'] = $thisprof;
    $row['organization'] = $inst;
    $row['additionalrecipients'] = $addl;
	$row['completed'] = $comp;
    }
    $rows[] = $row;
  }
  module_load_include('inc', 'quiz', 'quiz.pages');

 $quiztitle = "All Enrollment Report";
 $output .= $quiztitle;
 $output .= _quiz_export_link();

 if ($_GET['export']) {
  $csvarray = array('header' => header, 'rows' => $rows);
  $csvarray = $rows;
  #print_r($csvarray);
  convert_to_csv($csvarray, $quiztitle.'.csv', ',');
  } else {
  $output .= theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'tablesorter')));
  return $output;
  }
}


/**
 * Get stats for a single quiz. Maybe also for a single user.
 *
 * @param $vid
 *   quiz revision id
 *
 * @param $uid
 *   User id
 *
 * @return
 *   HTML page with charts/graphs


 SELECT nr.result_id as result_id, nr.time_end AS time_end, nr.nid AS nid, nr.time_start AS time_start, nr.score AS score, nr.vid AS vid, qa.result_id AS result_id, qa.question_nid AS question_nid, qa.question_vid AS question_vid, qa.is_correct AS is_correct, qa.number AS number FROM quiz_node_results nr INNER JOIN quiz_node_results_answers qa ON qa.result_id = nr.result_id WHERE (nr.vid = 97) AND (nr.nid = 57)

 */
function quiz_stats_get_all_answers_by_quiz_and_revision($nid, $vid) {
	$output = "";
	$quiztitle = _quiz_title($nid->nid);
	$output .= $quiztitle;
	$output .= _quiz_export_link();
   $results = _quiz_get_quiz_data($vid, $nid->nid);

	  if (empty($results)) {
		return '<p>' . t('No results found.') . '</p>';
	  }

	$header = $rows = array();


  	$headfields = "0";
	foreach ($results as $result) {
		if (user_access('access author stats')) {

		$row['email'] = $result['enrolleesemail'];
		$row['prof'] = $result['enrolleesprof'];
		$row['addl'] = $result['enrolleesaddl'];
		$row['inst'] = $result['enrolleesinst'];

		#$row['link'] = '<a href="/node/'. $result['nid'] .'/quiz/results/'. $result['result_id'].'/view">View</a>';
		$row['score'] = $result['score'];
		#print $result['correctanswers'] ."<br>";
		$answersarray = explode(" / ", $result['correctanswers']);


		if (count($answersarray) > $headfields) {
			$headfields = count($answersarray);
		}

		foreach ($answersarray as $key => $answer) {
			$row[$key] = $answer;
		}

		if ($result['time_end']) {
			$row['completed'] = date("F j, Y - g:i a", $result['time_end']);
		} else {
			$row['completed'] = "Not Completed";
		}
	}

	$rows[] = $row;
	}


	$header['email'] = t('E-mail');
	$header['profession'] = t('Profession');

    $header['additionalrecipients'] = t('Additional Recipients');
	$header['organization'] = t('Organization');
	#$header['link'] = t('Link');
	$header['score'] = t('Overall Score');
	$hf=1;

	while ($hf <= $headfields) {
		$header[$hf] = t('Q').$hf;
	$hf++;

	}

 $header['completed'] = t('Date Submitted');


  module_load_include('inc', 'quiz', 'quiz.pages');

  if ($_GET['export']) {
  $csvarray = array('header' => header, 'rows' => $rows);
  $csvarray = $rows;
  #print_r($csvarray);
  convert_to_csv($csvarray, $quiztitle.'.csv', ',');
  } else {
  $output .= theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'tablesorter')));
  return $output;
  }

}

function quiz_stats_get_aggregate_answers_by_quiz_and_revision($nid, $vid) {
	$output = "";
	$quiztitle = _quiz_title($nid->nid);
	$output .= $quiztitle;
	$output .= _quiz_export_link();
   #$results = _quiz_get_quiz_data($vid, $nid->nid);

   $totals =  _quiz_get_total_answers_by_question($vid, $nid->nid);

  # print_r($totals);

   $results = _quiz_get_correct_answer_info($vid, $nid->nid);


	  if (empty($results)) {
		return '<p>' . t('No results found.') . '</p>';
	  }

	$header = $rows = array();


  	$headfields = "0";
	$answersarray = $results;
	foreach ($answersarray as $answer) {
		#print_r($answer);
	}
	$i=1;

	#$row[$i] = "hey";
	$row['totenrolled'] = $totals->totalenrolled;
	$row['totecomplete'] = $totals->totalcompleted;
	#$row['avgscore'] = round($totals->averagescore, 2);
	$row['avgscore'] = $totals->totalenrolled/$totals->totalcompleted;
	$raughone['totenrolled'] = "";
	$raughone['totencomplete'] = "";
	$raughone['avgscore'] = "";
	$raugh['totenrolled'] = "";
	$raugh['totencomplete'] = "";
	$raugh['avgscore'] = "";
	$row['totcorlabel'] = "Total Correct";
	$raughone['totanslabel'] = "Total Answered";
	$raugh['totperclabel'] = "Percentage Correct";
	$tempcorrect = "";
	$temptotalanswers = "";
		foreach ($results as $key => $result) {

			$row[$key] = $result['totalcorrect'];

			$tempcorrect = $tempcorrect + $result['totalcorrect'];
			$temptotalanswers = $temptotalanswers + $result['totalanswers'];

			$percent = $result['totalcorrect']/$result['totalanswers'];
			$percent_friendly = number_format( $percent * 100, 2 ) . '%';
			$raughone[$key] = $result['totalanswers'];
			$raugh[$key] = round($percent_friendly, 2).'%';

		$i++;
		}
		#print $tempcorrect ."<br>";
		#print $temptotalanswers ."<br>";
		$row['avgscore'] = round($tempcorrect/$temptotalanswers, 2) * 100 . '%';
	#print_r($row);



	$rows[] = $row;
	$rows[] = $raughone;
	$rows[] = $raugh;
	if (count($answersarray) > $headfields) {
		$headfields = count($answersarray);
	}
	$hf=1;

	$header['en'] = t('# Enrolled');
	$header['cmpl'] = t('# Completed');
	$header['score'] = t('Overall % Correct');
	$header['totalslabels'] = t('');
	while ($hf <= $headfields) {
		$header[$hf] = t('Q').$hf;
	$hf++;

	}



  module_load_include('inc', 'quiz', 'quiz.pages');
  #print_r($results);
  if ($_GET['export']) {
  $csvarray = array('header' => header, 'rows' => $rows);
  $csvarray = $rows;
  #print_r($csvarray);
  convert_to_csv($csvarray, $quiztitle.'.csv', ',');
  } else {
  $output .= theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'tablesorter')));
  return $output;
  }
  /**/
}


/**
	* Get a list of all answers by quiz/revision.
	*
	* @param $vid
	*
	* @return
	*   A list of enrollees .
	*/
function _quiz_get_quiz_data($vid, $nid) {
	$results = array();
	$args = array();
	$query = db_select('quiz_node_results', 'nr')
		->fields('nr', array('time_end', 'nid', 'time_start', 'score', 'vid', 'result_id'))
		#->fields('qa', array('result_id', 'question_nid', 'question_vid', 'is_correct', 'number', 'result_answer_id'))
		;
	#$query->innerJoin('quiz_node_results_answers', 'qa', 'qa.result_id = nr.result_id');

	$query->condition('nr.vid', $vid);
	$query->condition('nr.nid', $nid);
	#$query->condition('nr.is_evaluated', '1');
	$quizzes = $query->execute();

	#$quizres = db_query('SELECT nid, time_start, score, vid FROM {quiz_node_results} nr INNER JOIN {quiz_node_results_answers} qa ON (qa.result_id = nr.result_id) WHERE nr.result_id = :result_id', array(':result_id' => $quiz->result_id));

	#$quizzes = $quizres->fetchObject();
	#print_r($quizres);

	foreach ($quizzes as $quiz) {
		#print $quiz->result_id;
		#print_r($quiz);
		#$questionsquery = array();
		$questionsquery = db_select('quiz_node_results', 'nr')
			->fields('nr', array('nid', 'time_start', 'score', 'vid'))
			->fields('qa', array('result_id', 'question_nid', 'question_vid', 'is_correct', 'number', 'result_answer_id'));
		$questionsquery->innerJoin('quiz_node_results_answers', 'qa', 'qa.result_id = nr.result_id');
		$questionsquery->condition('nr.result_id', $quiz->result_id);
		$questionsarray = $questionsquery->execute();

		#restructure the query to make more sense... try below on line 808

		#$questions = array();
		$answercorrect = array();

		$enrolleesresults = _quiz_get_enrollment_by_result($quiz->result_id);
		#print_r($enrolleesresults);
		foreach ($enrolleesresults as $enrollees) {
			if (trim($enrollees->email) == "") {
				$email = "Anonymous";
				$thisprof = "Anonymous";
				$addl = "Anonymous";
				$inst = "Anonymous";
			} else {
				$email = $enrollees->email;
				if ($enrollees->profession == 0) {
				$thisprof = "N/A";
				} else {
				$prof = taxonomy_term_load($enrollees->profession);
			    $thisprof = $prof->name;
				}
				$addl = $enrollees->additional_email;
				$inst = $enrollees->institution;
			}
			#print $enrollees->profession;
		}
		if (!empty($enrolleesresults)) {
			#while ($question = $quesres->fetchObject()) {
			foreach ($questionsarray as $question) {


				#$derp = quiz_question_get_report($quiz->question_nid, $quiz->question_vid, $quiz->result_id);
				#print_r($derp);
				#$hoo = _quiz_question_response_get_instance($quiz->result_id, NULL, NULL, $question->question_nid, $question->question_vid);
				#print_r($hoo);
				#_quiz_question_response_get_instance($result_id, NULL, NULL, $nid, $vid);

				/*
				function _quiz_question_response_get_instance($result_id, $question, $answer = NULL, $nid = NULL, $vid = NULL)
				*/
				#$qanswer = _quiz_question_response_get_instance($quiz->result_id, $derp);
				$qanswer = _quiz_question_response_get_instance($question->result_id, NULL, NULL, $question->question_nid, $question->question_vid);
					#print_r($qanswer->question);
					if ($qanswer->question->type == 'multichoice') {

						$query  = "
						SELECT *
						FROM {quiz_multichoice_user_answers} ua
						JOIN {quiz_multichoice_user_answer_multi} uam
						ON uam.user_answer_id = ua.id
						AND ua.result_id = :result_id
						AND ua.question_nid = :question_nid
						AND ua.question_vid = :question_vid";

						$arguments = array(
							':result_id' => $quiz->result_id,
							':question_nid' => $qanswer->question->nid,
							':question_vid' => $qanswer->question->vid
						);

						$res = db_query($query, $arguments);
						$user_answers = $res->fetchAll();
						#print_r($user_answers);
						foreach($user_answers as $user_answer){
							$answercorrect[] = _get_quiz_answer_text_mca($user_answer->answer_id);
							#$answercorrect[] = $user_answer->answer_id;
						}
						if (sizeof($user_answers) < 1) {
							$answercorrect[] = "0";
						}

					}

					if ($qanswer->question->type == 'long_answer') {

						$query  = "
						SELECT *
						FROM {quiz_long_answer_user_answers} ua
						WHERE ua.result_id = :result_id
						AND ua.question_nid = :question_nid
						AND ua.question_vid = :question_vid";

						$arguments = array(
							':result_id' => $quiz->result_id,
							':question_nid' => $qanswer->question->nid,
							':question_vid' => $qanswer->question->vid
						);

						$res = db_query($query, $arguments);
						$user_answers = $res->fetchAll();
						#print_r($user_answers);
						foreach($user_answers as $user_answer){
							$answercorrect[] = $user_answer->answer;
							#$answercorrect[] = $user_answer->answer_id;
						}
						if (sizeof($user_answers) < 1) {
							$answercorrect[] = "0";
						}

					}
			}

		#print_r($answercorrect);
		$results[$quiz->result_id] = (array) $quiz;
		$results[$quiz->result_id]['enrolleesemail'] = $email. "<!--". $quiz->result_id ."-->";
		$results[$quiz->result_id]['enrolleesprof'] = $thisprof;
		$results[$quiz->result_id]['enrolleesaddl'] = $addl;
		$results[$quiz->result_id]['enrolleesinst'] = $inst;
		$results[$quiz->result_id]['questions'] = implode(",", $questions);
		#print_r($answercorrect);
		#print "<br>";
		if( empty( $answercorrect ) )
							{
								 // list is empty.
								 #print_r($answercorrect);
								 #$quiz->result_id.'has an empty array';
							}
			$results[$quiz->result_id]['correctanswers'] = implode(" / ", $answercorrect);


	}
		}
	return $results;

}

function _quiz_get_correct_answer_info($vid, $nid) {
	$results = array();
	$args = array();
	$answercorrect = array();
	$query = db_select('quiz_node_results', 'nr')
		->fields('nr', array('time_end', 'nid', 'time_start', 'score', 'vid'))
		->fields('qa', array('result_id', 'question_nid', 'question_vid', 'number', 'result_answer_id'))
		;
	$query->innerJoin('quiz_node_results_answers', 'qa', 'qa.result_id = nr.result_id');

	$query->condition('nr.vid', $vid);
	$query->condition('nr.nid', $nid);
	$quizzes = $query->execute();



	foreach ($quizzes as $quiz) {

		$questionsquery = array();
		$questionsquery = db_select('quiz_node_results', 'nr')
			->fields('nr', array('time_end', 'nid', 'time_start', 'score', 'vid'))
			->fields('qa', array('result_id', 'question_nid', 'question_vid', 'number', 'result_answer_id'));
		$questionsquery->innerJoin('quiz_node_results_answers', 'qa', 'qa.result_id = nr.result_id');
		$questionsquery->condition('nr.result_id', $quiz->result_id);
		$questionsarray = $questionsquery->execute();

		$questions = array();
		$answercorrect = array();


			foreach ($questionsarray as $question) {

				$derp = quiz_question_get_report($question->question_nid, $question->question_vid, $quiz->result_id);
				$qanswer =  _quiz_question_response_get_instance($question->result_id, $derp);

					if ($qanswer->question->type == 'multichoice') {
						$tempcorrect = _quiz_get_correct_by_question($question->question_vid, $question->question_nid, $vid, $nid);
						$tempanswers = _quiz_get_totalanswers_by_question($question->question_vid, $question->question_nid, $vid, $nid);
						$answercorrect[$question->question_nid]['totalcorrect'] = $tempcorrect->totalcorrect;
						$answercorrect[$question->question_nid]['totalanswers'] = $tempanswers->totalanswers;
					}
			}

	}
	return $answercorrect;
}

function _quiz_get_correct_by_question($vid, $nid, $quizvid, $quiznid) {
	#print $nid."<br/ >";
	#print $vid."<br/ >";


	#$tcount = db_query('SELECT count(is_correct) as totalcorrect FROM {quiz_node_results_answers} WHERE is_correct = 1 and question_nid = :question_nid AND question_vid = :question_vid', array(':question_nid' => $nid, ':question_vid' => $vid));
	$tcount = db_query('SELECT count(is_correct) as totalcorrect FROM {quiz_node_results_answers} WHERE is_correct = 1 and question_nid = :question_nid AND question_vid = :question_vid and result_id in (select result_id from {quiz_node_results} WHERE nid = :quiz_nid AND vid = :quiz_vid)', array(':question_nid' => $nid, ':question_vid' => $vid,':quiz_nid' => $quiznid, ':quiz_vid' => $quizvid));

	$counts = $tcount->fetch();
	return $counts;
}

function _quiz_get_totalanswers_by_question($vid, $nid, $quizvid, $quiznid) {

	$tcount = db_query('SELECT count(result_answer_id) as totalanswers FROM {quiz_node_results_answers} WHERE question_nid = :question_nid AND answer_timestamp IS NOT NULL AND question_vid = :question_vid and result_id in (select result_id from {quiz_node_results} WHERE nid = :quiz_nid AND vid = :quiz_vid)', array(':question_nid' => $nid, ':question_vid' => $vid,':quiz_nid' => $quiznid, ':quiz_vid' => $quizvid));
	$counts = $tcount->fetch();
	return $counts;
}


function _quiz_get_total_enrollees($vid, $nid) {

	$tcount = db_query('SELECT count(time_start) as totalenrolled, count(time_end) as totalcompleted, AVG(score) as averagescore FROM {quiz_node_results} WHERE nid = :quiz_nid AND vid = :quiz_vid', array(':quiz_nid' => $nid, ':quiz_vid' => $vid));
	$counts = $tcount->fetch();

	return $counts;
}

function _quiz_get_total_answers_by_question($vid, $nid) {

	$tcount = db_query('SELECT count(time_start) as totalenrolled, count(time_end) as totalcompleted, AVG(score) as averagescore FROM {quiz_node_results} WHERE nid = :quiz_nid AND vid = :quiz_vid', array(':quiz_nid' => $nid, ':quiz_vid' => $vid));
	$counts = $tcount->fetch();

	return $counts;
}

function _get_quiz_answer_text_mca($aid) {

	#$res = db_query('SELECT answer,weight FROM {quiz_multichoice_answers} WHERE id = :aid', array(':aid' => $aid));
	#print $aid;
	$res = db_query('SELECT question_nid,question_vid,weight FROM {quiz_multichoice_answers} WHERE id = :aid', array(':aid' => $aid));
	$res_o = $res->fetchObject();
	#print_r($res_o);
	#print "SELECT question_nid,question_vid,weight FROM {quiz_multichoice_answers} WHERE id=".$vid;

	#print "SELECT * FROM quiz_multichoice_answers WHERE question_nid = ".$qid." AND question_vid = ".$vid." order by weight;";
	if ($res_o) {
		$all = array();
		$qid = $res_o->question_nid;
		$qvid = $res_o->question_vid;
		$allanswers = db_query('SELECT * FROM {quiz_multichoice_answers} WHERE question_nid = :question_nid AND question_vid = :question_vid order by weight', array(':question_nid' => $qid, ':question_vid' => $qvid));

		$counts = $allanswers->fetchCol();
		#print $qid .'<br />';
		#print_r($counts);
		#print '<br />';
		#print_r($allanswers);
		while ($answer = current($counts)) {
			#print $answer ." - ";
			if ($answer == $aid) {
				$getanswernumber = key($counts)+1;
				#print $answer ." should equal ". $aid ." count will be:". $getanswernumber ." -- selected<br />";
			} else {

			}
			next($counts);
		}


		return $getanswernumber;
	} else {
		#print_r($res_o);

		return $aid."No Answer";
		#return "No Answer";
	}
}
/**
 * Generates grade range chart
 *
 * @param $uid
 *   User id
 * @param $vid
 *   revision id of quiz node
 * @return
 *   array with chart and metadata
 */
function _get_quiz_grade_range_chart($vid, $uid = 0) {
  // @todo: make the ranges configurable
  $sql = 'SELECT SUM(score >= 0 && score < 20) AS zero_to_twenty,
    SUM(score >= 20 && score < 40) AS twenty_to_fourty,
    SUM(score >= 40 && score < 60) AS fourty_to_sixty,
    SUM(score >= 60 && score < 80) AS sixty_to_eighty,
    SUM(score >= 80 && score <= 100) AS eighty_to_hundred
    FROM {quiz_node_results}
    WHERE vid = :vid';
  $arg[':vid'] = $vid;
  if ($uid != 0) {
    $sql .= ' AND uid = :uid';
    $arg[':uid'] = $uid;
  }
  $range = db_query($sql, $arg)->fetch();
  $count = $range->zero_to_twenty + $range->twenty_to_fourty + $range->fourty_to_sixty + $range->sixty_to_eighty + $range->eighty_to_hundred;
  if ($count < 2) {
    return FALSE;
  }
  // Get the charts
  $chart = '<div id="quiz_top_scorers" class="quiz-stats-chart-space">';
  $chart .= theme('quiz_grade_range', array('range' => $range));
  $chart .= '</div>';
  // Return the chart with some meta data
  return array(
    'chart' => $chart,
    'title' => t('Distribution'),
    'explanation' => t('This chart shows the distribution of the scores on this quiz.'),
  );
}

/**
 * Generates the top scorers chart
 *
 * @param $vid
 *   revision id of quiz node
 *
 * @param $uid
 *   User id
 *
 * @return
 *   array with chart and metadata
 */
function _get_quiz_top_scorers_chart($vid, $uid = 0) {
  $top_scorers = array();
  $sql = 'SELECT name, score FROM {quiz_node_results} qnr
    LEFT JOIN {users} u ON (u.uid = qnr.uid)
    WHERE vid = :vid';
  $arg[':vid'] = $vid;
  if ($uid != 0) {
    $sql .= ' AND qnr.uid = :uid';
    $arg[':uid'] = $uid;
  }
  $sql .= ' ORDER by score DESC LIMIT 10';
  $results = db_query($sql, $arg);
  while ($result = $results->fetchAssoc()) {
    $key = $result['name'] . '-' . $result['score'];
    $top_scorers[$key] = $result;
  }
  if (count($top_scorers) == 0) {
    return FALSE;
  }
  $chart = '<div id="quiz_top_scorers" class="quiz-stats-chart-space">';
  $chart .= theme('quiz_top_scorers', array('scorer' => $top_scorers));
  $chart .= '</div>';
  return array(
    'chart' => $chart,
    'title' => t('Top scorers'),
    'explanation' => t('This chart shows which quiz takers have the highest scores.'),
  );
}

/**
 * Generates a chart showing the status for all registered responses to a quiz
 *
 * @param $vid
 *   revision id of quiz node
 *
 * @param $uid
 *   User id
 *
 * @return
 *   HTML to render to chart/graph
 */
function _get_quiz_status_chart($vid, $uid = 0) {
  // get the pass rate of the given quiz by vid
  $pass_rate = db_query("SELECT pass_rate FROM {quiz_node_properties} WHERE vid = :vid", array(':vid' => $vid))->fetchField();
  if (!$pass_rate) {
    return;
  }
  // get the count value of results row above and below pass rate
  $quiz = db_query("SELECT SUM(score >= $pass_rate) AS no_pass, SUM(score < $pass_rate) AS no_fail, SUM(is_evaluated = 0) AS no_incomplete FROM {quiz_node_results} WHERE vid = :vid", array(':vid' => $vid))
    ->fetchAssoc();
  if (($quiz['no_pass'] + $quiz['no_fail'] + $quiz['no_incomplete']) < 1) {
    return FALSE; // no sufficient data
  }
  // generates quiz status chart 3D pie chart
  $chart = '<div id="get_quiz_status_chart" class="quiz-stats-chart-space">';
  $chart .= theme('get_quiz_status_chart', array('quiz' => $quiz));
  $chart .= '</div>';
  return array(
    'chart' => $chart,
    'title' => t('Status'),
    'explanation' => t('This chart shows the status for all attempts made to answer this revision of the quiz.'),
  );
}

/**
 * Generates chart showing how often the quiz has been taken the last days
 *
 * @param $uid
 *   user id
 * @param $vid
 *   revision id of quiz
 * @return
 *   HTML to display chart
 */
function _get_date_vs_takeup_count_chart($vid, $uid = 0) {
  $start = 0;
  $end = 30;
  $takeup = array();
  $sql = "SELECT COUNT(result_id) AS count,
            DATE_FORMAT(FROM_UNIXTIME(time_start), '%Y-%m-%e') AS date
            FROM {quiz_node_results}
            WHERE vid = :vid AND time_start > (UNIX_TIMESTAMP(NOW()) - (86400*$end))";
  $sql_args[':vid'] = $vid;
  if ($uid != 0) {
    $sql .= " AND uid = :uid";
    $sql_args[':uid'] = $uid;
  }
  $sql .= " GROUP BY date ORDER BY time_start ASC";
  $results = db_query($sql, $sql_args);
  $res_o = $results->fetchAll();
  if ($res_o) {
    $chart = '<div id="date_vs_takeup_count" class="quiz-stats-chart-space">';
    // wrapping the chart output with div for custom theming.
    $chart .= theme('date_vs_takeup_count', array('takeup' => $res_o));
    // generate date vs takeup count line chart
    $chart .= '</div>';
    return array(
      'chart' => $chart,
      'title' => t('Activity'),
      'explanation' => t('This chart shows how many times the quiz has been taken each day over the last !days days.', array('!days' => $end)),
    );
  }
}

/**
 * Computes quiz attempt count
 *
 * @param $vid
 *   quiz node id
 * @param $uid
 *   user id for filtering
 * @return
 *   Integer containing number of times that a quiz has
 *   been attended, can be filtered by uid.
 */
function _quiz_get_attempt_count($nid, $uid = 0) {
  $sql = 'SELECT COUNT(result_id) FROM {quiz_node_results} WHERE nid = :nid';
  $arg[':nid'] = $nid;
  if ($uid != 0) {
    $sql .= ' AND uid = :uid';
    $arg[':uid'] = $uid;
  }
  return db_query($sql, $arg)->fetchField();
}

function _quiz_get_attempt_count_with_vid($nid, $vid) {
 # $sql = 'SELECT COUNT(result_id) FROM {quiz_node_results} WHERE nid = :nid and vid = :vid';
 # $arg[':nid'] = $nid;
 # $arg[':vid'] = $vid;

  $sql = 'SELECT COUNT(ua.result_id) FROM {quiz_node_results} ua
						JOIN {quiz_demographics_user_answers} uad
						ON uad.result_id = ua.result_id
						AND ua.nid = :nid
						AND ua.vid = :vid';
  $arg[':nid'] = $nid;
  $arg[':vid'] = $vid;
  #if ($uid != 0) {
  #  $sql .= ' AND uid = :uid';
  #  $arg[':uid'] = $uid;
  #}
  return db_query($sql, $arg)->fetchField();
}

/* ALL RESULTS */

function _quiz_get_all_results_for_quiz($nid, $uid = 0) {
  $sql = 'SELECT * FROM {quiz_node_results} WHERE nid = :nid';
  $arg[':nid'] = $nid;
  if ($uid != 0) {
    $sql .= ' AND uid = :uid';
    $arg[':uid'] = $uid;
  }
  return db_query($sql, $arg)->fetchField();
}


// Theme functions

/**
 * Theme function for quiz grade range chart generates Chart using CHART API function
 *
 * @param $range
 *   array containg quiz results data structure
 *
 * @return
 *   HTML to render/display chart
 */
function theme_quiz_grade_range($variables) {
  $range = $variables['range'];

  $max = max((array) $range);
  $count = array_sum((array) $range);

  $chart = array(
    '#type' => 'chart',
    '#chart_type' => 'column',
    '#title' => t('Score distribution'),
  );
  $chart['range'] = array(
    '#type' => 'chart_data',
    '#title' => t('% of quiz takers'),
    '#data' => array(
      round(($range->zero_to_twenty / $count) * 100),
      round(($range->twenty_to_fourty / $count) * 100),
      round(($range->fourty_to_sixty / $count) * 100),
      round(($range->sixty_to_eighty / $count) * 100),
      round(($range->eighty_to_hundred / $count) * 100),
    ),
  );
  $chart['xaxis'] = array(
    '#title' => t('Score'),
    '#type' => 'chart_xaxis',
    '#labels' => array(
      '0-20%',
      '20-40%',
      '40-60%',
      '60-80%',
      '80-100%',
    ),
  );
  $chart['yaxis'] = array(
    '#title' => t('Grade Distribution'),
    '#type' => 'chart_yaxis',
    '#max' => 100,
  );
  return drupal_render($chart);
}

/**
 * generates a chart of quiz top scorers
 *
 * @param $attendees
 *   array containing quiz data structure
 *
 * @return
 *   HTML to render quiz top scorers chart
 */
function theme_quiz_top_scorers($variables) {
  $attendees = array_reverse($variables['scorer']);
  $chart = array(
    '#type' => 'chart',
    '#title' => t('Quiz Top Scorers'),
    '#chart_type' => 'column',
  );
  $chart['bar_data'] = array(
    '#type' => 'chart_data',
    '#title' => 'Result',
  );
  foreach ($attendees as $attendee) {
    $chart['bar_data']['#data'][] = (float) $attendee['score'];
    $labels[] = $attendee['name'] . ' ' . $attendee['score'] . '%';

  }
  $chart['yaxis'] = array(
    '#title' => t('Score') . ' (%)',
    '#type' => 'chart_yaxis',
    '#max' => 100,
  );
  $chart['xaxis'] = array(
    '#title' => 'Quiz Attendees',
    '#type' => 'chart_xaxis',
    '#labels' => $labels,

  );
  return drupal_render($chart);
}

/**
 * generates quiz status chart number of pass, fail, incomplete
 *
 * @param $quiz
 *   array of quiz data structure
 *
 * @return
 *   HTML to render quiz status chart
 */
function theme_get_quiz_status_chart($variables) {
  $quiz = $variables['quiz'];
  $quiz['total'] = $quiz['no_pass'] + $quiz['no_fail'] + $quiz['no_incomplete'];

  $chart = array(
    '#type' => 'chart',
    '#title' => t('Quiz results'),
    '#chart_type' => 'pie',
  );

  $chart['pie_data'] = array(
    '#type' => 'chart_data',
    '#title' => t('Result'),
    '#labels' => array('Pass', 'Incomplete', 'Fail'),
    '#data' => array(
      (int) $quiz['no_pass'],
      (int) $quiz['no_incomplete'],
      (int) $quiz['no_fail']
    ),
  );
  return drupal_render($chart);
}

/**
 * Generates date vs takeup count chart
 *
 * @param $takeup
 *   Array quiz data structure
 * @return
 *   HTML to render/display chart
 */
function theme_date_vs_takeup_count($variables) {
  $chart = array(
    '#type' => 'chart',
    '#title' => t('Activity by date'),
    '#chart_type' => 'line',
  );

  $chart['line_data'] = array(
    '#type' => 'chart_data',
    '#title' => t('Result'),
    '#labels' => array('Something'),
  );

  $chart['line_data']['#data'] = array();
  $labels = array();
  foreach ($variables['takeup'] as $point) {
    $chart['line_data']['#data'][] = (int) $point->count;
    $labels[] = $point->date;
  }

  $chart['xaxis'] = array(
    '#type' => 'chart_xaxis',
    '#labels' => $labels,
  );
  $chart['yaxis'] = array(
    '#title' => 'Participants',
    '#type' => 'chart_yaxis',
  );
  return drupal_render($chart);
}

/**
 * Generates table of results from quiz data structure.
 *
 * @param @results
 *   Array containing quiz results data structure
 * @return
 *   HTML table containing quiz title, author, questions count, attempt count
 */
function theme_quiz_stats_get_basic_stats($variables) {
  $results = $variables['results'];
  if (empty($results)) {
    return '<p>' . t('No questions were found.') . '</p>';
  }
  $header = $rows = array();
  $header = array(
    'title' => t('Quiz'),
    'creator' => t('Author'),
    'created' => t('Created'),
  );
  if (user_access('access author stats')) {
    $header['questions_count'] = t('Questions');
    $header['attempt_count'] = t('Attempts');
  }
  $p = drupal_get_path('module', 'quiz_stats');
  $chart_icon = theme('image', array('path' => "$p/chart.png", 'width' => t('Charts'), 'height' => t('See charts')));
  foreach ($results as $result) {
    $title_link = user_access('access author stats') ? 'node/' . $result['nid'] . '/quiz/statistics' : 'user/' . arg(1) . '/stats/';
    $row = array(
      'title' => l($chart_icon . ' ' . $result['title'], $title_link, array('html' => TRUE)),
      'creator' => l($result['name'], 'user/' . $result['uid']),
      'created' => format_date($result['created'], 'short'),
    );
    if (user_access('access author stats')) {
      $row['questions_count'] = quiz_get_number_of_questions($result['vid']);
      $row['attempt_count'] = _quiz_get_attempt_count_with_vid($result['nid'],$result['vid']);
    }
    $rows[] = $row;
  }
  module_load_include('inc', 'quiz', 'quiz.pages');
  $cc = '<em>' . t('Chart icon from !url', array('!url' => 'pinvoke.com')) . '</em>';
  return theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'tablesorter'))) . $cc;
}

/**
 * Generates table of results from quiz data structure.
 *
 * @param @results
 *   Array containing quiz results data structure
 * @return
 *   HTML table containing quiz title, author, questions count, attempt count

 SOME STUFF IN quiz_stats.module for callbacks to work. - JG
 */
function theme_quiz_stats_get_all_enrollment($variables) {
  $results = $variables['results'];
  #print_r($variables);
  if (empty($results)) {
    return '<p>' . t('No questions were found.') . '</p>';
  }
  $header = $rows = array();
  $header = array(
    'title' => t('Quiz Title'),
    'creator' => t('Author'),
    'created' => t('Created'),
  );
  if (user_access('access author stats')) {
    $header['questions_count'] = t('Questions');
    $header['attempt_count'] = t('Attempts');
  }
  $p = drupal_get_path('module', 'quiz_stats');
  $chart_icon = theme('image', array('path' => "$p/chart.png", 'width' => t('Charts'), 'height' => t('See charts')));
  foreach ($results as $result) {
    $title_link = user_access('access author stats') ? 'node/' . $result['nid'] . '/quiz/all-enrollment' : 'user/' . arg(1) . '/stats/';
    $row = array(
      'title' => l(_quiz_title($result['nid']) . ' ', $title_link, array('html' => TRUE)),
      'creator' => l($result['name'], 'user/' . $result['uid']),
      'created' => format_date($result['created'], 'short'),
    );
    if (user_access('access author stats')) {
      $row['questions_count'] = quiz_get_number_of_questions($result['vid']);
      $row['all_results'] = _quiz_get_all_results_for_quiz($result['nid']);

    }
    $rows[] = $row;
  }
  module_load_include('inc', 'quiz', 'quiz.pages');
  $cc = '<em>' . t('CUSTOM DATASET') . '</em>';
  return theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'tablesorter'))) . $cc;
}

/**
 * Generates table of results from quiz data structure.
 *
 * @param @results
 *   Array containing quiz results data structure
 * @return
 *   HTML table containing quiz title, author, questions count, attempt count

 SOME STUFF IN quiz_stats.module for callbacks to work. - JG
 */
function theme_quiz_stats_get_all_answers_by_quiz($variables) {
  $results = $variables['results'];
  #print_r($variables);
  if (empty($results)) {
    return '<p>' . t('No questions were found.') . '</p>';
  }
  $header = $rows = array();
  $header = array(
    'title' => t('Quiz Title'),
    'creator' => t('Author'),
    'created' => t('Created'),
  );
  if (user_access('access author stats')) {
    $header['questions_count'] = t('Questions');
    $header['attempt_count'] = t('Attempts');
  }
  $p = drupal_get_path('module', 'quiz_stats');
  $chart_icon = theme('image', array('path' => "$p/chart.png", 'width' => t('Charts'), 'height' => t('See charts')));
  foreach ($results as $result) {
    $title_link = user_access('access author stats') ? 'node/' . $result['nid'] . '/quiz/all-answers' : 'user/' . arg(1) . '/stats/';
    $row = array(
      'title' => l(_quiz_title($result['nid']) . ' ', $title_link, array('html' => TRUE)),
      'creator' => l($result['name'], 'user/' . $result['uid']),
      'created' => format_date($result['created'], 'short'),
    );
    if (user_access('access author stats')) {
      $row['questions_count'] = quiz_get_number_of_questions($result['vid']);
      #$row['all_results'] = _quiz_get_all_results_for_quiz($result['nid']);
	  $row['all_results'] = _quiz_get_attempt_count_with_vid($result['nid'],$result['vid']);
    }
    $rows[] = $row;
  }
  module_load_include('inc', 'quiz', 'quiz.pages');
  $cc = '<em>' . t('All Answers by Quiz') . '</em>';
  return theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'tablesorter'))) . $cc;
}

function _quiz_title($nid) {
	$quiz = node_load($nid);
	$quiztitle = $quiz->title.' GFF - '. $quiz->field_fast_fact_number['und'][0]['value'];
	return $quiztitle;
}
function _quiz_export_link() {
	$output = ' <a href="?export=this" class="pull-right btn">Export</a>';
	return $output;
}
function convert_to_csv($input_array, $output_file_name, $delimiter)
{
    /** open raw memory as file, no need for temp files */
    $temp_memory = fopen('php://memory', 'w');
    /** loop through array  */
    foreach ($input_array as $line) {
        /** default php csv handler **/
        fputcsv($temp_memory, $line, $delimiter);
    }
    /** rewrind the "file" with the csv lines **/
    fseek($temp_memory, 0);
    /** modify header to be downloadable csv file **/
    header('Content-Type: application/csv');
    header('Content-Disposition: attachement; filename="' . $output_file_name . '";');
    /** Send file to browser for download */
    fpassthru($temp_memory);
}


function _get_quiz_answer_text_mca_old($aid) {

	#$res = db_query('SELECT answer,weight FROM {quiz_multichoice_answers} WHERE id = :aid', array(':aid' => $aid));

	$res = db_query('SELECT question_nid,question_vid,weight FROM {quiz_multichoice_answers} WHERE id = :aid', array(':aid' => $aid));
	$res_o = $res->fetch();
	#print_r($res_o);

	if ($res_o) {
		$all = array();
		$qid = $res_o->question_nid;
		$qvid = $res_o->question_vid;
		$allanswers = db_query('SELECT id FROM {quiz_multichoice_answers} WHERE question_nid = :question_nid AND question_vid = :question_vid order by weight', array(':question_nid' => $qid, ':question_vid' => $qvid));
		$counts = $allanswers->fetchCol();
		#print_r($counts);

		#print_r($all);
		while ($answer = current($counts)) {
			if ($answer == $aid) {
				$getanswernumber =  key($counts);
			}
			next($counts);
		}


		return $getanswernumber+1;
	} else {
		#print_r($res_o);
		return "No Answer";
	}
}
