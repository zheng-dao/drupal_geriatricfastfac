<?php
/**
 * Defines the classes necessary for a demographics question.
 *
 * @file
 */

/**
 * Extension of QuizQuestion.
 */
class DemographicsQuestion extends QuizQuestion {
	/**
	 * Implementation of saveNodeProperties
	 *
	 * @see QuizQuestion#saveNodeProperties($is_new)
	 */
	public function saveNodeProperties($is_new = FALSE) {
		//if ($is_new || $this->node->revision == 1) {
		//    $id = db_insert('quiz_demographics_node')
		//        ->fields(array(
		//            'nid' => $this->node->nid,
		//            'vid' => $this->node->vid,
		//        ))
		//        ->execute();
		//}
		//else {
		//    db_update('quiz_demographics_node')
		//        ->fields(array())
		//        ->condition('nid', $this->node->nid)
		//        ->condition('vid', $this->node->vid)
		//        ->execute();
		//}
	}

	/**
	 * Implementation of validateNode
	 *
	 * @see QuizQuestion#validateNode($form)
	 */
	public function validateNode(array &$form) {
		// This space intentionally left blank. :)
	}

	/**
	 * Implementation of delete
	 *
	 * @see QuizQuestion#delete($only_this_version)
	 */
	public function delete($only_this_version = FALSE) {
		parent::delete($only_this_version);

		$delete_ans = db_delete('quiz_demographics_user_answers');
		$delete_ans->condition('question_nid', $this->node->nid);

		$delete_node = db_delete('quiz_demographics_node');
		$delete_node->condition('nid', $this->node->nid);

		if ($only_this_version) {
			$delete_ans->condition('question_vid', $this->node->vid);
			$delete_node->condition('vid', $this->node->vid);
		}

		$delete_ans->execute();
		$delete_node->execute();
	}

	/**
	 * Implementation of getNodeProperties
	 *
	 * @see QuizQuestion#getNodeProperties()
	 */
	public function getNodeProperties() {
		if (isset($this->nodeProperties)) {
			return $this->nodeProperties;
		}

		return parent::getNodeProperties();
	}

	/**
	 * Implementation of getNodeView
	 *
	 * @see QuizQuestion#getNodeView()
	 */
	public function getNodeView() {
		return parent::getNodeView();
	}

	/**
	 * Implementation of getAnsweringForm
	 *
	 * @see QuizQuestion#getAnsweringForm($form_state, $result_id)
	 */
	public function getAnsweringForm(array $form_state = NULL, $result_id) {
		$element = parent::getAnsweringForm($form_state, $result_id);
		//$element['#theme'] = 'demographics_answering_form';
		$element['#no_report'] = TRUE;

		$element['email'] = array(
			'#type' => 'textfield',
			'#title' => t('Your Email'),
			//'#description' => t('Enter your email address'),
			'#default_value' => '',
			'#size' => 60,
			'#maxlength' => 256,
			'#required' => FALSE,
			'#attributes' => array('autocomplete' => 'off'),
		);

		$element['profession'] = array(
			'#type' => 'select',
			'#title' => t('Your Profession'),
			//'#description' => t('Select your profession'),
			'#default_value' => variable_get('profession', 0),
			'#options' => array(
				0 => t('Select One:'),
				33 => t('Physician'),
				34 => t('Nurse'),
				35 => t('Pharmacist'),
				36 => t('Psychologist'),
				37 => t('Social Worker'),
				38 => t('Rehab Therapist'),
				39 => t('Physician Assistant'),
				40 => t('Other'),
			),
		);

		$element['institution'] = array(
			'#type' => 'textfield',
			'#title' => t('Institution'),
			//'#description' => t('Enter the institution with which you are associated'),
			'#default_value' => '',
			'#size' => 60,
			'#maxlength' => 256,
			'#required' => FALSE,
			'#attributes' => array('autocomplete' => 'off'),
		);

		$element['additional_email'] = array(
			'#type' => 'textfield',
			'#title' => t('Additional/3rd Party Email'),
			//'#description' => t('Enter your instructor\'s email address'),
			'#default_value' => '',
			'#size' => 60,
			'#maxlength' => 256,
			'#required' => FALSE,
			'#attributes' => array('autocomplete' => 'off'),
		);

		if (isset($result_id)) {
			$response = new DemographicsResponse($result_id, $this->node);
			$element['#default_value'] = $response->getResponse();
		}
		
		$element['#validate'][] = 'demographics_answer_form_validate';

		return $element;
	}

	/**
	 * Question response validator.
	 */
	public function demographics_answer_form_validate(array &$form, array &$form_state = NULL) {
		if (is_null($form_state['values']['question'][$this->node->nid])) {
			form_set_error('', t('You must provide an answer.'));
		}
	}

	///**
	// * Implementation of getBodyFieldTitle
	// *
	// * @see QuizQuestion#getBodyFieldTitle()
	// */
	//public function getBodyFieldTitle() {
	//    return t('Demographic Information');
	//}

	/**
	 * Implementation of getCreationForm
	 *
	 * @see QuizQuestion#getCreationForm($form_state)
	 */
	public function getCreationForm(array &$form_state = NULL) {
		return $form;
	}

	/**
	 * Implementation of getMaximumScore
	 *
	 * @see QuizQuestion#getMaximumScore()
	 */
	public function getMaximumScore() {
		return 0;
	}
}

/**
 * Extension of QuizQuestionResponse
 */
class DemographicsResponse extends QuizQuestionResponse {
	/**
	 * Constructor
	 */
	public function __construct($result_id, stdClass $question_node, $answer = NULL) {
		parent::__construct($result_id, $question_node, $answer);

		if (!isset($answer)) {
			$r = db_query('SELECT question_vid, question_nid, result_id, email, profession, institution, additional_email FROM {quiz_demographics_user_answers} WHERE question_nid = :qnid AND question_vid = :qvid AND result_id = :rid', array(':qnid' => $question_node->nid, ':qvid' => $question_node->vid, ':rid' => $result_id))->fetch();
			
			if (!empty($r)) {
				$this->email = $r->email;
				$this->institution = $r->institution;
				$this->profession = $r->profession;
				$this->additional_email = $r->additional_email;
			}
		} else {
			//dprint_r($answer);
			$this->email = '';
			$this->institution = '';
			$this->profession = '';
			$this->additional_email = '';
		}
	}

	/**
	 * Implementation of save
	 *
	 * @see QuizQuestionResponse#save()
	 */
	public function save() {
		//watchdog('demographics save', '%view', array('%view' => print_r($this, TRUE)));
		//dprint_r($this->answer);
		$this->answer_id = db_insert('quiz_demographics_user_answers')
			->fields(array(
				'question_nid' => $this->question->nid,
				'question_vid' => $this->question->vid,
				'result_id' => $this->result_id,
				'email' => $this->answer['email'],
				'profession' => $this->answer['profession'],
				'institution' => $this->answer['institution'],
				'additional_email' => $this->answer['additional_email'],
			))
			->execute();
	}

	/**
	 * Implementation of delete
	 *
	 * @see QuizQuestionResponse#delete()
	 */
	public function delete() {
		db_delete('quiz_demographics_user_answers')
			->condition('question_nid', $this->question->nid)
			->condition('question_vid', $this->question->vid)
			->condition('result_id', $this->result_id)
			->execute();
	}

	/**
	 * Implementation of score
	 *
	 * @see QuizQuestionResponse#score()
	 */
	public function score() {
		return 0;
	}

	/**
	 * Implementation of getResponse
	 *
	 * @see QuizQuestionResponse#getResponse()
	 */
	public function getResponse() {
		//watchdog('demographics.classes.inc - getResponse()', '%view', array('%view' => '$this->answer = ' . dprint_r($this->answer, TRUE)));
		return $this->answer;
	}

	/**
	 * Implementation of getReportFormResponse
	 */
	public function getReportFormResponse() {
		//watchdog('demographics.classes.inc - getReportFormResponse()', '%view', array('%view' => '$this = ' . dprint_r($this, TRUE)));
		$data = array();

		//$data[] = array(
		//    'choice' => '',
		//    'attempt' => '',
		//    'correct' => '',
		//    'score' => $this->getScore(),
		//    'question_annotation' => '',
		//    'solution' => ','
		//);

		return $data;
	}
	
	///**
	//* Implementation of getReportFormSubmit
	//*
	//* @see QuizQuestionResponse#getReportFormSubmit()
	//*/
	//public function getReportFormSubmit() {
	//    return 'demographics_report_submit';
	//}
	
	///**
	//* Implements QuizQuestionResponse::getReportFormValidate().
	//*/
	public function getReportFormValidate(&$element, &$form_state) {
		if ($element['email']['#value'] !== '') {
			if ($element['profession']['#value'] == '' || $element['institution']['#value'] == '') {
				form_error($element['profession'], t('If you want your results emailed to you, you must also input your profession and institution.'));
			}
		}
	
		//    watchdog('demographics stuff inside getReportFormValidate()', '%view', array('%view' => print_r($element, TRUE)));
		//    // Check to make sure that entered score is not higher than max allowed score.
		//    //if ($element['email']['#value'] > $this->question->max_score) {
		//    //    form_error($element['score'], t('The score needs to be a number between 0 and @max', array('@max' => $this->question->max_score)));
		//    //}
	}
}