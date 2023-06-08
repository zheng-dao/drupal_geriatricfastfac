<?php

/**
	* Action to add questions to the current quiz.
	*/
function quiz_add_default_question_to_quiz($question) {
	$quiz = node_load(arg(1));
	$quiz_questions = quiz_get_questions($quiz->nid, $quiz->vid);

	// Do not add a question if it's already been added (selected in an earlier checkbox)
	foreach ($quiz_questions as $q) {
		if ($question->vid == $q->vid) {
			return FALSE;
		}
	}

	// Otherwise let's add a relationship!
	$question->quiz_nid = $quiz->nid;
	$question->quiz_vid = $quiz->vid;
	_quiz_question_get_instance($question)->saveRelationships();
	quiz_update_max_score_properties(array($quiz->vid));
}

/**
	* Implement hook_rules_action_info().
	*/
function quiz_rules_action_info() {
	return array(
		'quiz_add_default_question_to_quiz' => array(
			'label' => t('Add a question to a quiz'),
			'group' => t('Quiz'),
			//'parameter' => array(
			//    'length' => array(
			//        'type' => 'integer',
			//        'label' => t('The length of the returned string'),
			//        'description' => t('Enter a number for the length of the hashed string that will be created.'),
			//    ),
			//    'algorithm' => array(
			//        'type' => 'text',
			//        'label' => t('Algorithm'),
			//        'description' => t('Select a hash algorithm.'),
			//        'options list' => 'my_module_algorithm_options',
			//        'restriction' => 'input',
			//    ),
			//),
			//'provides' => array(
			//    'hashed_string' => array(
			//        'type' => 'text',
			//        'label' => t('Hashed string'),
			//    ),
			//),
		),
	);
}