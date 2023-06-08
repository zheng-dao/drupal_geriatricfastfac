<?php

/**
	* Long answer classes.
	*
	* @file
	*  Classes modelling the long answer question and the long answer question response
	*/

/**
	* Extension of QuizQuestion.
	*/
class LongAnswerQuestion extends QuizQuestion {
	/**
	* Implementation of saveNodeProperties
	* @see QuizQuestion#saveNodeProperties($is_new)
	*/
	public function saveNodeProperties($is_new = FALSE) {
		watchdog('long_answer.classes.inc - saveNodeProperties()', '$this->node = %view', array('%view' => print_r($this->node, TRUE)));

		if (!isset($this->node->annotation)) {
			$this->node->annotation = 'No annotation';
		}
	}

	/**
	* Implementation of validateNode
	*
	* @see QuizQuestion#validateNode($form)
	*/
	public function validateNode(array &$form) {}

	/**
	* Implementation of delete
	*
	* @see QuizQuestion#delete($only_this_version)
	*/
	public function delete($only_this_version = FALSE) {
		if ($only_this_version) {
			db_delete('quiz_long_answer_user_answers')
				->condition('question_nid', $this->node->nid)
				->condition('question_vid', $this->node->vid)
				->execute();
			db_delete('quiz_long_answer_node_properties')
				->condition('nid', $this->node->nid)
				->condition('vid', $this->node->vid)
				->execute();
		} else {
			db_delete('quiz_long_answer_user_answers')
				->condition('question_nid', $this->node->nid)
				->execute();
			db_delete('quiz_long_answer_node_properties')
				->condition('nid', $this->node->nid)
				->execute();
		}

		parent::delete($only_this_version);
	}

	/**
	* Implementation of getNodeProperties
	*
	* @see QuizQuestion#getNodeProperties()
	*/
	public function getNodeProperties() {
		if (isset($this->nodeProperties) && !empty($this->nodeProperties)) {
			return $this->nodeProperties;
		}

		$props = parent::getNodeProperties();

		if (isset($props['annotation'])) {
			$this->node->annotation = $props['annotation'];
		}

		$this->nodeProperties = $props;

		return $props;
	}

	/**
	* Implementation of getAnweringForm
	*
	* @see QuizQuestion#getAnsweringForm($form_state, $result_id)
	*/
	public function getAnsweringForm(array $form_state = NULL, $result_id) {
		$element = parent::getAnsweringForm($form_state, $result_id);

		$element['answer'] = array(
			'#type' => 'textarea',
			'#title' => t('Answer'),
			//'#description' => t('Enter your answer here. If you need more space, click on the grey bar at the bottom of this area and drag it down.'),
			'#rows' => 15,
			'#cols' => 60
		);

		if (isset($result_id)) {
			$response = new LongAnswerResponse($result_id, $this->node);
			$element['#default_value'] = $response->getResponse();
		}

		return $element;
	}

	/**
	* Question response validator.
	*/
	public function getAnsweringFormValidate(array &$form, array &$form_state = NULL) {
		if ($form_state['values']['question'][$this->node->nid] == '') {
			form_set_error('', t('You must provide an answer.'));
		}
	}

	/**
	* Implementation of getCreationForm
	*
	* @see QuizQuestion#getCreationForm($form_state)
	*/
	public function getCreationForm(array &$form_state = NULL) {
		$form['annotation'] = array(
			'#type' => 'text_format',
			'#title' => t('Question annotation'),
			'#default_value' => $this->node->annotation,
			'#required' => FALSE,
			'#description' => t('Add an annotation to the question that can be used for grading or comparison by the user.'),
			'#weight' => 2,
		);

		return $form;
	}

	/**
	* Implementation of getMaximumScore
	*
	* @see QuizQuestion#getMaximumScore()
	*/
	public function getMaximumScore() {
		// Long answer scores are not handled by our system. Those are graded manually if at all
		return 0;
	}
}

/**
	* Extension of QuizQuestionResponse
	*/
class LongAnswerResponse extends QuizQuestionResponse {
	/**
	* Get all scores that have not yet been evaluated.
	*
	* @param $count
	*  Number of items to return (default: 50).
	* @param $offset
	*  Where in the results we should start (default: 0).
	*
	* @return
	*  Array of objects describing answered questions. Each object will have result_id, question_nid, and question_vid.
	*/
	public static function fetchAllAnswers($count = 50, $offset = 0) {
		$query = db_select('quiz_long_answer_user_answers', 'a')
			->fields('a', array('result_id', 'question_nid', 'question_vid'))
			->fields('r', array('title'))
			->fields('qnr', array('time_end', 'time_start', 'uid'))
			->join('node_revision', 'r', 'a.question_vid = r.vid')
			->join('quiz_node_results', 'qnr', 'a.result_id = qnr.result_id')
			->join('node', 'n', 'qnr.nid = n.nid');

		$results = $query->execute();
		$answers = array();

		foreach ($results as $row) {
			$answers[] = $row;
		}

		return $answers;
	}

	/**
	* Given a quiz, return a list of all of the answers.
	*
	* @param $nid
	*  Node ID for the quiz to check.
	* @param $vid
	*  Version ID for the quiz to check.
	* @param $count
	*  Number of items to return (default: 50).
	* @param $offset
	*  Where in the results we should start (default: 0).
	*
	* @return
	*  Indexed array of result IDs that need to be scored.
	*/
	public static function fetchAnswersByQuestion($nid, $vid, $count = 50, $offset = 0) {
		$results = db_query('SELECT result_id FROM {quiz_long_answer_user_answers} WHERE question_nid = :question_nid AND question_vid = :question_vid', array(':question_nid' => $nid, ':question_vid' => $vid));
		$answers = array();

		foreach ($results as $row) {
			$answers[] = $row->result_id;
		}

		return $answers;
	}

	/**
	* ID of the answer.
	*/
	protected $answer_id = 0;

	/**
	* Constructor
	*/
	public function __construct($result_id, stdClass $question_node, $answer = NULL) {
		parent::__construct($result_id, $question_node, $answer);

		if (!isset($answer)) {
			// Question has been answered already. We fetch the answer data from the database.
			$r = db_query('SELECT answer_id, answer, question_vid, question_nid, result_id FROM {quiz_long_answer_user_answers} WHERE question_nid = :qnid AND question_vid = :qvid AND result_id = :rid', array(':qnid' => $question_node->nid, ':qvid' => $question_node->vid, ':rid' => $result_id))->fetch();

			if (!empty($r)) {
				$this->answer = $r->answer;
				$this->score = 0;
				$this->answer_id = $r->answer_id;
			}
		} else {
			$this->answer = $answer;
			$this->evaluated = FALSE;
		}
	}

	/**
	* Implementation of save
	*
	* @see QuizQuestionResponse#save()
	*/
	public function save() {
		//watchdog('long_answer.classes.inc - save()', '$this = %view', array('%view', dprint_r($this, TRUE)));
		$this->answer_id = db_insert('quiz_long_answer_user_answers')
			->fields(array(
				'answer' => $this->answer,
				'question_nid' => $this->question->nid,
				'question_vid' => $this->question->vid,
				'result_id' => $this->result_id
			))
			->execute();
	}

	/**
	* Implementation of delete
	*
	* @see QuizQuestionResponse#delete()
	*/
	public function delete() {
		db_delete('quiz_long_answer_user_answers')
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
		return $this->answer;
	}

	/**
	* Implementation of getReportFormResponse
	*/
	public function getReportFormResponse() {
		$data = array();
		//watchdog('long_answer.classes.inc - getReportFormResponse()', '$this[\'quizQuestion\'] = %view', array('%view' => dprint_r($this->quizQuestion->node->annotation, TRUE)));

		$data[] = array(
			'attempt' => $this->answer,
		);

		return $data;
	}

	public function getReportFormQuestionAnnotation() {
		//watchdog('long_answer.classes.inc - getReportFormQuestionAnnotation()', '$this->quizQuestion = %view', array('%view' => print_r($this->quizQuestion, TRUE)));
		return array(
			'#title' => t('Correct answer'),
			'#type' => 'text_format',
			'#default_value' => isset($this->node->annotation['value']) ? $this->node->annotation['value'] : 'Annotation not supplied',
			'#format' => 'full_html',
			'#attributes' => array('class' => array('quiz-report-question-annotation')),
		);
	}
}
