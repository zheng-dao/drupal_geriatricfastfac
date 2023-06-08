<?php

/**
	* The main classes for the multichoice question type.
	*
	* These inherit or implement code found in quiz_question.classes.inc.
	*
	* Sponsored by: Norwegian Centre for Telemedicine
	* Code: falcon
	*
	* Based on:
	* Other question types in the quiz framework.
	*
	*
	*
	* @file
	* Question type, enabling the creation of multiple choice and multiple answer questions.
	*/

/**
	* Extension of QuizQuestion.
	*/
class MultichoiceQuestion extends QuizQuestion {
	// No forgiveness in our module...
	///**
	//* Forgive some possible logical flaws in the user input.
	//*/
	//private function forgive() {
	//    if ($this->node->choice_multi == 1) {
	//        for ($i = 0; isset($this->node->alternatives[$i]); $i++) {
	//            $short = &$this->node->alternatives[$i];

	//            // If the scoring data doesn't make sense, use the data from the "correct" checkbox to set the score data
	//            if ($short['score_if_chosen'] == $short['score_if_not_chosen'] || !is_numeric($short['score_if_chosen']) || !is_numeric($short['score_if_not_chosen'])) {
	//                if (!empty($short['correct'])) {
	//                    $short['score_if_chosen'] = 1;
	//                    $short['score_if_not_chosen'] = 0;
	//                } else {
	//                    if (variable_get('multichoice_def_scoring', 0) == 0) {
	//                        $short['score_if_chosen'] = -1;
	//                        $short['score_if_not_chosen'] = 0;
	//                    } elseif (variable_get('multichoice_def_scoring', 0) == 1) {
	//                        $short['score_if_chosen'] = 0;
	//                        $short['score_if_not_chosen'] = 1;
	//                    }
	//                }
	//            }
	//        }
	//    } else {
	//        // For questions with one, and only one, correct answer, there will be no points awarded for alternatives not chosen.
	//        for ($i = 0; isset($this->node->alternatives[$i]); $i++) {
	//            $short = &$this->node->alternatives[$i];
	//            $short['score_if_not_chosen'] = 0;

	//            if (isset($short['correct']) && $short['correct'] == 1 && !_quiz_is_int($short['score_if_chosen'], 1)) {
	//                $short['score_if_chosen'] = 1;
	//            }
	//        }
	//    }
	//}

	// No warnings either.
	///**
	//* Warn the user about possible user errors
	//*/
	//private function warn() {
	//    // Count the number of correct answers
	//    $num_corrects = 0;

	//    for ($i = 0; isset($this->node->alternatives[$i]); $i++) {
	//        $alt = &$this->node->alternatives[$i];

	//        if ($alt['score_if_chosen'] > $alt['score_if_not_chosen']) {
	//            $num_corrects++;
	//        }
	//    }

	//    if ($num_corrects == 1 && $this->node->choice_multi == 1 || $num_corrects > 1 && $this->node->choice_multi == 0) {
	//        $link_options = array();

	//        if (isset($_GET['destination'])) {
	//            $link_options['query'] = array('destination' => $_GET['destination']);
	//        }

	//        $go_back = l(t('go back'), 'node/' . $this->node->nid . '/edit', $link_options);

	//        if ($num_corrects == 1) {
	//            drupal_set_message(t('Your question allows multiple answers. Only one of the alternatives have been marked as correct. If this wasn\'t intended please !go_back and correct it.', array('!go_back' => $go_back)), 'warning');
	//        } else {
	//            drupal_set_message(t('Your question doesn\'t allow multiple answers. More than one of the alternatives have been marked as correct. If this wasn\'t intended please !go_back and correct it.', array('!go_back' => $go_back)), 'warning');
	//        }
	//    }
	//}

	/**
	* Run check_markup() on the field of the specified choice alternative
	* @param $alternativeIndex
	*  The index of the alternative in the alternatives array.
	* @param $field
	*  The name of the field we want to check markup on
	* @param $check_user_access
	*  Whether or not to check for user access to the filter we're trying to apply
	* @return HTML markup
	*/
	private function checkMarkup($alternativeIndex, $field, $check_user_access = FALSE) {
		$alternative = $this->node->alternatives[$alternativeIndex];

		return check_markup($alternative[$field]['value'], $alternative[$field]['format']);
	}

	/**
	* Implementation of save
	*
	* Stores the question in the database.
	*
	* @param is_new if - if the node is a new node...
	* (non-PHPdoc)
	* @see sites/all/modules/quiz-HEAD/question_types/quiz_question/QuizQuestion#save()
	*/
	public function saveNodeProperties($is_new = FALSE) {
		$is_new = $is_new || $this->node->revision == 1;

		//// Before we save we forgive some possible user errors
		//$this->forgive();

		//// We also add warnings on other possible user errors
		//$this->warn();

		if ($is_new) {
			$id = db_insert('quiz_multichoice_properties')
				->fields(array(
					'nid' => $this->node->nid,
					'vid' => $this->node->vid,
					'choice_multi' => $this->node->choice_multi,
					'choice_random' => $this->node->choice_random,
					'choice_boolean' => $this->node->choice_boolean,
				))
				->execute();

			// TODO: utilize the benefit of multiple insert of DBTNG
			for ($i = 0; isset($this->node->alternatives[$i]); $i++) {
				//dprint_r($this->node->alternatives[$i]);
				if (drupal_strlen($this->node->alternatives[$i]['answer']['value']) > 0) {
					$this->insertAlternative($i);
				}
			}
		} else {
			db_update('quiz_multichoice_properties')
				->fields(array(
					'choice_multi' => $this->node->choice_multi,
					'choice_random' => $this->node->choice_random,
					'choice_boolean' => $this->node->choice_boolean,
				))
				->condition('nid', $this->node->nid)
				->condition('vid', $this->node->vid)
				->execute();

			// We fetch ids for the existing answers belonging to this question
			// We need to figure out if an existing alternative has been changed or deleted.
			$res = db_query('SELECT id FROM {quiz_multichoice_answers} WHERE question_nid = :nid AND question_vid = :vid', array(':nid' => $this->node->nid, ':vid' => $this->node->vid));

			// We start by assuming that all existing alternatives needs to be deleted
			$ids_to_delete = array();

			while ($res_o = $res->fetch()) {
				$ids_to_delete[] = $res_o->id;
			}

			for ($i = 0; isset($this->node->alternatives[$i]); $i++) {
				$short = $this->node->alternatives[$i];

				if (drupal_strlen($this->node->alternatives[$i]['answer']['value']) > 0) {
					// If new alternative
					if (!is_numeric($short['id'])) {
						$this->insertAlternative($i);
					} else {
						// If existing alternative
						$this->updateAlternative($i);
						// Make sure this alternative isn't deleted
						$key = array_search($short['id'], $ids_to_delete);
						$ids_to_delete[$key] = FALSE;
					}
				}
			}

			foreach ($ids_to_delete as $id_to_delete) {
				if ($id_to_delete) {
					db_delete('quiz_multichoice_answers')
						->condition('id', $id_to_delete)
						->execute();
				}
			}
		}

		if (db_query('SELECT COUNT(annotation) FROM quiz_question_properties WHERE nid = :nid AND vid = :vid', array(':nid' => $this->node->nid, ':vid' => $this->node->vid)) > 0) {
			db_update('quiz_question_properties')
				->fields(array(
					'annotation' => $this->annotation,
				))
				->condition('nid', $this->node->nid)
				->condition('vid', $this->node->vid)
				->execute();
		} else {
			db_insert('quiz_question_properties')
				->fields(array(
					'nid' => $this->node->nid,
					'vid' => $this->node->vid,
					'annotation' => $this->annotation,
				))
				->execute();
		}
	}

	function _normalizeAlternative($alternatives) {
		$copy = $alternatives;

		// Answer
		if (is_array($alternatives['answer']) && isset($alternatives['answer']['value'])) {
			$copy['answer'] = $alternatives['answer']['value'];
		}

		// Answer format
		if (is_array($alternatives['answer']) && isset($alternatives['answer']['format'])) {
			$copy['answer_format'] = $alternatives['answer']['format'];
		}

		return $copy;
	}

	/**
	* Helper function. Saves new alternatives
	*
	* @param $i
	*  The alternative index
	*/
	private function insertAlternative($i) {
	$alternatives = $this->_normalizeAlternative($this->node->alternatives[$i]);
	db_insert('quiz_multichoice_answers')
		->fields(array(
			'answer' => $alternatives['answer'],
			'answer_format' => $alternatives['answer_format'],
			'score_if_chosen' => $alternatives['score_if_chosen'],
			'score_if_not_chosen' => $alternatives['score_if_not_chosen'],
			'question_nid' => $this->node->nid,
			'question_vid' => $this->node->vid,
			'weight' => isset($alternatives['weight']) ? $alternatives['weight'] : $i,
		))
		->execute();
	}

	/**
	* Helper function. Updates existing alternatives
	*
	* @param $i
	*  The alternative index
	*/
	private function updateAlternative($i) {
		$alternatives = $this->_normalizeAlternative($this->node->alternatives[$i]);
		db_update('quiz_multichoice_answers')
			->fields(array(
				'answer' => $alternatives['answer'],
				'answer_format' => $alternatives['answer_format'],
				'score_if_chosen' => $alternatives['score_if_chosen'],
				'score_if_not_chosen' => $alternatives['score_if_not_chosen'],
				'weight' => isset($alternatives['weight']) ? $alternatives['weight'] : $i,
			))
			->condition('id', $alternatives['id'])
			->condition('question_nid', $this->node->nid)
			->condition('question_vid', $this->node->vid)
			->execute();
	}

	/**
	* Implementation of validate
	*
	* QuizQuestion#validate()
	*/
	public function validateNode(array &$form) {
		if ($this->node->choice_multi == 0) {
			$found_one_correct = FALSE;

			for ($i = 0; (isset($this->node->alternatives[$i]) && is_array($this->node->alternatives[$i])); $i++) {
				$short = $this->node->alternatives[$i];

				if (drupal_strlen($this->checkMarkup($i, 'answer')) < 1) {
					continue;
				}

				if ($short['correct'] == 1) {
					if ($found_one_correct) {
						// We don't display an error message here since we allow alternatives to be partially correct
					} else {
						$found_one_correct = TRUE;
					}
				}
			}

			if (!$found_one_correct) {
				form_set_error('choice_multi', t('You have not marked any alternatives as correct. If there are no correct alternatives you should allow multiple answers.'));
			}
		} else {
			for ($i = 0; isset($this->node->alternatives[$i]); $i++) {
				$short = $this->node->alternatives[$i];

				if (strlen($this->checkMarkup($i, 'answer')) < 1) {
					continue;
				}

				if ($short['score_if_chosen'] < $short['score_if_not_chosen'] && $short['correct']) {
					form_set_error("alternatives][$i][score_if_not_chosen", t('The alternative is marked as correct, but gives more points if you don\'t select it.'));
				} elseif ($short['score_if_chosen'] > $short['score_if_not_chosen'] && !$short['correct']) {
					form_set_error("alternatives][$i][score_if_chosen", t('The alternative is not marked as correct, but gives more points if you select it.'));
				}
			}
		}
	}

	/**
	* Implementation of delete
	*
	* @see QuizQuestion#delete()
	*/
	public function delete($only_this_version = FALSE) {
		$delete_properties = db_delete('quiz_multichoice_properties')->condition('nid', $this->node->nid);
		$delete_answers = db_delete('quiz_multichoice_answers')->condition('question_nid', $this->node->nid);
		$delete_results = db_delete('quiz_multichoice_user_answers')->condition('question_nid', $this->node->nid);

		if ($only_this_version) {
			$delete_properties->condition('vid', $this->node->vid);
			$delete_answers->condition('question_vid', $this->node->vid);
			$delete_results->condition('question_vid', $this->node->vid);
		}

		// Delete from table quiz_multichoice_user_answer_multi
		if ($only_this_version) {
			$user_answer_id = db_query('SELECT id FROM {quiz_multichoice_user_answers} WHERE question_nid = :nid AND question_vid = :vid', array(':nid' => $this->node->nid, ':vid' => $this->node->vid))->fetchCol();
		} else {
			$user_answer_id = db_query('SELECT id FROM {quiz_multichoice_user_answers} WHERE question_nid = :nid', array(':nid' => $this->node->nid))->fetchCol();
		}

		if (!empty($user_answer_id)) {
			db_delete('quiz_multichoice_user_answer_multi')
				->condition('user_answer_id', $user_answer_id, 'IN')
				->execute();
			db_delete('quiz_question_properties')
				->condition('nid', $this->node->nid)
				->condition('vid', $this->node->vid)
				->execute();
		}

		$delete_properties->execute();
		$delete_answers->execute();
		$delete_results->execute();
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

		$res_a = db_query('SELECT choice_multi, choice_random, choice_boolean FROM {quiz_multichoice_properties} WHERE nid = :nid AND vid = :vid', array(':nid' => $this->node->nid, ':vid' => $this->node->vid))->fetchAssoc();

		if (is_array($res_a)) {
			$props = array_merge($props, $res_a);
		}

		if (isset($props['annotation'])) {
			$this->node->annotation = $props['annotation'];
		}

		// Load the answers
		$res = db_query('SELECT id, answer, answer_format, score_if_chosen, score_if_not_chosen, weight
				FROM {quiz_multichoice_answers} WHERE question_nid = :question_nid AND question_vid = :question_vid
				ORDER BY weight', array(':question_nid' => $this->node->nid, ':question_vid' => $this->node->vid));
		$props['alternatives'] = array(); // init array so it can be iterated even if empty

		while ($res_arr = $res->fetchAssoc()) {
			//print 'multichoice.classes.inc Line 387: ' . dprint_r($res_arr);
			$props['alternatives'][] = array(
				'id' => $res_arr['id'],
				'answer' => array(
					'value' => $res_arr['answer'],
					'format' => $res_arr['answer_format'],
				),
				'score_if_chosen' => $res_arr['score_if_chosen'],
				'score_if_not_chosen' => $res_arr['score_if_not_chosen'],
				'weight' => $res_arr['weight'],
			);
		}

		$this->nodeProperties = $props;

		return $props;
	}

	/**
	* Implementation of getNodeView
	*
	* @see QuizQuestion#getNodeView()
	*/
	public function getNodeView() {
		$content = parent::getNodeView();

		if ($this->node->choice_random) {
			$this->shuffle($this->node->alternatives);
		}

		// Hide this table to prevent redundancies on the quiz report form
		//$content['answers'] = array(
		//    '#markup' => theme('multichoice_answer_node_view', array('alternatives' => $this->node->alternatives, 'show_correct' => $this->viewCanRevealCorrect())),
		//    '#weight' => 2,
		//);

		return $content;
	}

	/**
	* Generates the question element.
	*
	* This is called whenever a question is rendered, either
	* to an administrator or to a quiz taker.
	*/
	public function getAnsweringForm(array $form_state = NULL, $result_id) {
		$element = parent::getAnsweringForm($form_state, $result_id);
		//$form['#theme'] = 'multichoice_answering_form';
		// We use an array looking key to be able to store multiple answers in tries.
		// At the moment all user answers have to be stored in tries. This is something we plan to fix in quiz 5.x.
		$element['#theme'] = 'multichoice_alternative';

		if (isset($result_id)) {
			// This question has already been answered. We load the answer.
			$response = new MultichoiceResponse($result_id, $this->node);
		}

		for ($i = 0; isset($this->node->alternatives[$i]); $i++) {
			$short = $this->node->alternatives[$i];
			$answer_markup = check_markup($short['answer']['value'], $short['answer']['format']);
			if (drupal_strlen($answer_markup) > 0) {
				$element['user_answer']['#options'][$short['id']] = $answer_markup;
			}
		}

		if ($this->node->choice_random) {
			// We save the choice order so that the order will be the same in the answer report
			$element['choice_order'] = array(
				'#type' => 'hidden',
				'#value' => implode(',', $this->shuffle($element['user_answer']['#options'])),
			);
		}

		if ($this->node->choice_multi) {
			$element['user_answer']['#type'] = 'checkboxes';
			$element['user_answer']['#title'] = t('Choose all that apply');

			if (isset($response)) {
				if (is_array($response->getResponse())) {
					$element['#default_value'] = $response->getResponse();
				}
			}
		} else {
			$element['user_answer']['#type'] = 'radios';
			$element['user_answer']['#title'] = t('Choose one');

			if (isset($response)) {
				$selection = $response->getResponse();

				if (is_array($selection)) {
					$element['user_answer']['#default_value'] = array_pop($selection);
				}
			}
		}

		$element['hidden_annotation'] = array(
			'#type' => 'hidden',
			'#value' => $this->annotation
		);

		return $element;
	}

	/**
	* Custom shuffle function. It keeps the array key - value relationship intact
	*
	* @param array $array
	* @return unknown_type
	*/
	private function shuffle(array &$array) {
		$newArray = array();
		$toReturn = array_keys($array);
		shuffle($toReturn);

		foreach ($toReturn as $key) {
			$newArray[$key] = $array[$key];
		}

		$array = $newArray;

		return $toReturn;
	}

	/**
	* Implementation of getCreationForm
	*
	* @see QuizQuestion#getCreationForm()
	*/
	public function getCreationForm(array &$form_state = NULL) {
		$form = array();
		$type = node_type_get_type($this->node);
		// We add #action to the form because of the use of ajax
		$options = array();
		$get = $_GET;
		unset($get['q']);

		if (!empty($get)) {
			$options['query'] = $get;
		}

		$action = url('node/add/' . $type->type, $options);

		if (isset($this->node->nid)) {
			$action = url('node/' . $this->node->nid . '/edit', $options);
		}

		$form['#action'] = $action;

		drupal_add_tabledrag('multichoice-alternatives-table', 'order', 'sibling', 'multichoice-alternative-weight');

		$form['alternatives'] = array(
			'#type' => 'fieldset',
			'#title' => t('Answer options'),
			'#collapsible' => TRUE,
			'#collapsed' => FALSE,
			'#weight' => -4,
			'#tree' => TRUE,
		);

		// Get the nodes settings, users settings or default settings
		$default_settings = $this->getDefaultAltSettings();

		$form['annotation'] = array(
			'#type' => 'text_format',
			'#title' => t('Question annotation'),
			'#default_value' => $this->node->annotation,
			'#required' => FALSE,
			'#description' => t('Add an annotation to the question that can be used for grading or comparison by the user.'),
			'#weight' => 2,
		);

		$form['alternatives']['settings'] = array(
			'#type' => 'fieldset',
			'#title' => t('Settings'),
			'#collapsible' => TRUE,
			'#collapsed' => FALSE,
			'#description' => t('Your settings will be remembered.'),
			'#weight' => 30,
		);

		$form['alternatives']['settings']['choice_multi'] = array(
			'#type' => 'checkbox',
			'#disabled' => TRUE,
			'#title' => t('Multiple answers'),
			'#description' => t('Allow any number of answers(checkboxes are used). If this box is not checked, one, and only one answer is allowed(radiobuttons are used).'),
			'#default_value' => $default_settings['choice_multi'],
			'#parents' => array('choice_multi'),
		);

		$form['alternatives']['settings']['choice_random'] = array(
			'#type' => 'checkbox',
			'#title' => t('Random order'),
			'#description' => t('Present alternatives in random order when @quiz is being taken.', array('@quiz' => QUIZ_NAME)),
			'#default_value' => $default_settings['choice_random'],
			'#parents' => array('choice_random'),
		);

		$form['alternatives']['settings']['choice_boolean'] = array(
			'#type' => 'checkbox',
			'#disabled' => TRUE,
			'#title' => t('Simple scoring'),
			'#description' => t('Give max score if everything is correct. Zero points otherwise.'),
			'#default_value' => $default_settings['choice_boolean'],
			'#parents' => array('choice_boolean'),
		);

		$form['alternatives']['#theme'][] = 'multichoice_creation_form';
		$i = 0;

		// choice_count might be stored in the form_state after an ajax callback
		if (isset($form_state['values']['op']) && $form_state['values']['op'] == t('Add choice')) {
			$form_state['choice_count'] ++;
		} else {
			$form_state['choice_count'] = max(variable_get('multichoice_def_num_of_alts', 5), isset($this->node->alternatives) ? count($this->node->alternatives) : 0);
		}

		$form['alternatives']['#prefix'] = '<div class="clear-block" id="multichoice-alternatives-wrapper">';
		$form['alternatives']['#suffix'] = '</div>';
		$form['alternatives']['#theme'] = array('multichoice_alternative_creation_table');

		for ($i = 0; $i < $form_state['choice_count']; $i++) {
			$short = isset($this->node->alternatives[$i]) ? $this->node->alternatives[$i] : NULL;
			$form['alternatives'][$i] = array(
				'#type' => 'container',
				'#collapsible' => TRUE,
				'#collapsed' => TRUE,
			);

			if (is_array($short)) {
				if ($short['score_if_chosen'] == $short['score_if_not_chosen']) {
					$correct_default = isset($short['correct']) ? $short['correct'] : FALSE;
				} else {
					$correct_default = $short['score_if_chosen'] > $short['score_if_not_chosen'];
				}
			} else {
				$correct_default = FALSE;
			}

			$form['alternatives'][$i]['correct'] = array(
				'#type' => 'checkbox',
				'#default_value' => $correct_default,
				'#attributes' => array('onchange' => 'Multichoice.refreshScores(this, ' . variable_get('multichoice_def_scoring', 0) . ')'),
			);

			// We add id to be able to update the correct alternatives if the node is updated, without destroying existing answer reports
			$form['alternatives'][$i]['id'] = array(
				'#type' => 'value',
				'#value' => $short['id'],
			);

			$form['alternatives'][$i]['answer'] = array(
				'#type' => 'text_format',
				'#default_value' => $short['answer']['value'],
				'#required' => $i < 2,
				'#format' => isset($short['answer']['format']) ? $short['answer']['format'] : NULL,
				'#rows' => 2,
			);

			$form['alternatives'][$i]['advanced'] = array(
				'#type' => 'fieldset',
				'#title' => t('Advanced options'),
				'#collapsible' => TRUE,
				'#collapsed' => TRUE,
			);

			$default_value = isset($this->node->alternatives[$i]['score_if_chosen']) ? $this->node->alternatives[$i]['score_if_chosen'] : 0;

			$form['alternatives'][$i]['advanced']['score_if_chosen'] = array(
				'#type' => 'textfield',
				'#title' => t('Score if chosen'),
				'#size' => 4,
				'#maxlength' => 4,
				'#default_value' => $default_value,
				'#description' => t("This score is added to the user's total score if the user chooses this alternative."),
				'#attributes' => array(
					'onkeypress' => 'Multichoice.refreshCorrect(this)',
					'onkeyup' => 'Multichoice.refreshCorrect(this)',
					'onchange' => 'Multichoice.refreshCorrect(this)'
				),
				'#parents' => array('alternatives', $i, 'score_if_chosen')
			);

			$default_value = $short['score_if_not_chosen'];

			if (!isset($default_value)) {
				$default_value = '0';
			}

			$form['alternatives'][$i]['advanced']['score_if_not_chosen'] = array(
				'#type' => 'textfield',
				'#title' => t('Score if not chosen'),
				'#size' => 4,
				'#maxlength' => 4,
				'#default_value' => $default_value,
				'#description' => t("This score is added to the user's total score if the user doesn't choose this alternative. Only used if multiple answers are allowed."),
				'#attributes' => array(
					'onkeypress' => 'Multichoice.refreshCorrect(this)',
					'onkeyup' => 'Multichoice.refreshCorrect(this)',
					'onchange' => 'Multichoice.refreshCorrect(this)'
				),
				'#parents' => array('alternatives', $i, 'score_if_not_chosen')
			);

			$form['alternatives'][$i]['weight'] = array(
				'#type' => 'textfield',
				'#size' => 2,
				'#attributes' => array('class' => array('multichoice-alternative-weight')),
				'#default_value' => isset($this->node->alternatives[$i]['weight']) ? $this->node->alternatives[$i]['weight'] : $i,
			);
		}

		$form['alternatives']['multichoice_add_alternative'] = array(
			'#type' => 'button',
			'#value' => t('Add answer option'),
			'#ajax' => array(
				'method' => 'replace',
				'wrapper' => 'multichoice-alternatives-wrapper',
				'callback' => 'multichoice_add_alternative_ajax_callback',
			),
			'#weight' => 20,
		);

		$form['#attached']['js'] = array(
			drupal_get_path('module', 'multichoice') . '/multichoice.js',
		);

		return $form;
	}

	/**
	* Helper function provding the default settings for the creation form.
	*
	* @return
	*  Array with the default settings
	*/
	private function getDefaultAltSettings() {
		// If the node is being updated the default settings are those stored in the node
		if (isset($this->node->nid)) {
			$settings['choice_multi'] = $this->node->choice_multi;
			$settings['choice_random'] = $this->node->choice_random;
			$settings['choice_boolean'] = $this->node->choice_boolean;
		} else {
		// The user is creating his first multichoice node
			$settings['choice_multi'] = 0;
			$settings['choice_random'] = 0;
			$settings['choice_boolean'] = 0;
		}

		return $settings;
	}

	///**
	//* Fetches the users default settings for the creation form
	//*
	//* @return
	//*  The users default node settings
	//*/
	//private function getUserSettings() {
	//    global $user;
	//    $res = db_query('SELECT choice_multi, choice_boolean, choice_random FROM {quiz_multichoice_user_settings} WHERE uid = :uid', array(':uid' => $user->uid))->fetchAssoc();

	//    if ($res) {
	//        return $res;
	//    }

	//    return FALSE;
	//}

	///**
	//* Fetches the users default settings from the creation form
	//*/
	//private function saveUserSettings() {
	//    global $user;
	//    db_merge('quiz_multichoice_user_settings')
	//        ->key(array('uid' => $user->uid))
	//        ->fields(array(
	//            'choice_random' => $this->node->choice_random,
	//            'choice_multi' => $this->node->choice_multi,
	//            'choice_boolean' => $this->node->choice_boolean,
	//        ))
	//        ->execute();
	//}

	/**
	* Implementation of getMaximumScore.
	*
	* @see QuizQuestion#getMaximumScore()
	*/
	public function getMaximumScore() {
		// Our multiple choice questions only have a max score of 1
		return 1;
	}

	/**
	* Question response validator.
	*/
	public function getAnsweringFormValidate(array &$form, array &$form_state = NULL) {
		if (!$this->node->choice_multi && is_null($form_state['values']['question'][$this->node->nid]['answer']['user_answer'])) {
			form_set_error('', t('You must provide an answer.'));
		}
	}
}

/**
	* Extension of QuizQuestionResponse
	*/
class MultichoiceResponse extends QuizQuestionResponse {
	/**
	* ID of the answers.
	*/
	protected $user_answer_ids;
	protected $choice_order;

	/**
	* Constructor.
	*
	* @param type $result_id
	* @param stdClass $question_node
	* @param type $values
	*   Form state values.
	*/
	public function __construct($result_id, stdClass $question_node, $values = NULL) {
		parent::__construct($result_id, $question_node, $values['user_answer']);

		//watchdog('multichoice.classes.inc - __construct()', '$values = %view', array('%view' => print_r($values, TRUE)));

		$this->user_answer_ids = array();

		if (isset($values['user_answer'])) {
			if (!is_array($values['user_answer'])) {
				// Account for single-select
				$values['user_answer'] = array($values['user_answer']);
			}

			if (isset($values['choice_order'])) {
				$this->choice_order = $values['choice_order'];
			}

			unset($values['choice_order']);

			if (isset($values['user_answer']) && is_array($values['user_answer'])) {
				foreach ($values['user_answer'] as $answer_id) {
					$this->user_answer_ids[] = $answer_id;
					$this->answer = $this->user_answer_ids; //@todo: Stop using user_answer_ids and only use answer instead...
				}
			} elseif (isset($values['user_answer'])) {
				$this->user_answer_ids[] = $values['user_answer'];
			}
		} else {
			// We load the answer from the database
			$res = db_query('SELECT answer_id FROM {quiz_multichoice_user_answers} ua LEFT OUTER JOIN {quiz_multichoice_user_answer_multi} uam ON(uam.user_answer_id = ua.id) WHERE ua.result_id = :result_id AND ua.question_nid = :question_nid AND ua.question_vid = :question_vid', array(':result_id' => $result_id, ':question_nid' => $this->question->nid, ':question_vid' => $this->question->vid));

			while ($res_o = $res->fetch()) {
				$this->user_answer_ids[] = $res_o->answer_id;
			}
		}
	}

	/**
	* Implementation of save
	*
	* @see QuizQuestionResponse#save()
	*/
	public function save() {
		$user_answer_id = db_insert('quiz_multichoice_user_answers')
			->fields(array(
				'result_id' => $this->result_id,
				'question_vid' => $this->question->vid,
				'question_nid' => $this->question->nid,
				'choice_order' => $this->choice_order
			))
			->execute();

		$query = db_insert('quiz_multichoice_user_answer_multi')
			->fields(array('user_answer_id', 'answer_id'));

		for ($i = 0; $i < count($this->user_answer_ids); $i++) {
			if ($this->user_answer_ids[$i]) {
				$query->values(array($user_answer_id, $this->user_answer_ids[$i]));
			}
		}

		$query->execute();
	}

	/**
	* Implementation of delete
	*
	* @see QuizQuestionResponse#delete()
	*/
	public function delete() {
		$user_answer_id = array();
		$query = db_query('SELECT id FROM {quiz_multichoice_user_answers} WHERE question_nid = :nid AND question_vid = :vid AND result_id = :result_id', array(':nid' => $this->question->nid, ':vid' => $this->question->vid, ':result_id' => $this->result_id));

		while ($user_answer = $query->fetch()) {
			$user_answer_id[] = $user_answer->id;
		}

		if (!empty($user_answer_id)) {
			db_delete('quiz_multichoice_user_answer_multi')
				->condition('user_answer_id', $user_answer_id, 'IN')
				->execute();
		}

		db_delete('quiz_multichoice_user_answers')
			->condition('result_id', $this->result_id)
			->condition('question_nid', $this->question->nid)
			->condition('question_vid', $this->question->vid)
			->execute();
	}

	/**
	* Implementation of score
	*
	* @return uint
	*
	* @see QuizQuestionResponse#score()
	*/
	public function score() {
		//watchdog('MULTICHOICE Scoring', 'max_score = %max, alternatives = %alt', array('%max' => $this->getMaxScore(), '%alt' => print_r($this->user_answer_ids, TRUE)));
		//if ($this->question->choice_boolean || $this->isAllWrong()) {
		//    $score = $this->getMaxScore();

		//    foreach ($this->question->alternatives as $key => $alt) {
		//        if (in_array($alt['id'], $this->user_answer_ids)) {
		//            if ($alt['score_if_chosen'] <= $alt['score_if_not_chosen']) {
		//                $score = 0;
		//            }
		//        } else {
		//            if ($alt['score_if_chosen'] > $alt['score_if_not_chosen']) {
		//                $score = 0;
		//            }
		//        }
		//    }
		//} else {
		$score = 0;

		foreach ($this->question->alternatives as $key => $alt) {
			if (in_array($alt['id'], $this->user_answer_ids)) {
				$score += $alt['score_if_chosen'];
			//} else {
			//    $score += $alt['score_if_not_chosen'];
			}
		}
		//}

		return $score;
	}

	/**
	* If all answers in a question are wrong
	*
	* @return boolean
	*  TRUE if all answers are wrong. False otherwise.
	*/
	public function isAllWrong() {
		foreach ($this->question->alternatives as $key => $alt) {
			if ($alt['score_if_chosen'] > 0 || $alt['score_if_not_chosen'] > 0) {
				return FALSE;
			}
		}

		return TRUE;
	}

	/**
	* Implementation of getResponse
	*
	* @return answer
	*
	* @see QuizQuestionResponse#getResponse()
	*/
	public function getResponse() {
		return $this->user_answer_ids;
	}

	/**
	* Implementation of getReportFormResponse
	*/
	public function getReportFormResponse() {
		$this->orderAlternatives($this->question->alternatives);
		$simple_scoring = $this->question->choice_boolean;
		$data = array();

		foreach ($this->question->alternatives as $alternative) {
			$chosen = in_array($alternative['id'], $this->user_answer_ids);
			#$not = $chosen ? '' : 'not_';
			$choice = $chosen ? $alternatives['score_if_chosen'] : $alternatives['score_if_not_chosen'];

			#$alternatives['score_if_chosen']
			#$alternatives['score_if_not_chosen']

			$icon = '';
			if ($alternative['score_if_chosen'] > 0) {	// If the current answer is correct
				if ($chosen) {
					$icon = quiz_icon('correct');	// The selected answer is correct
				} else {
					$icon = quiz_icon('should');	// The selected answer should have been chosen
				}
			} else {	// If the current answer is incorrect
				if ($chosen) {
					$icon = quiz_icon('incorrect');	// The selected answer is incorrect
				}
			}

			$data[] = array(
				'choice' => check_markup($alternative['answer']['value'], $alternative['answer']['format']),
				//'attempt' => $alternative['score_if_chosen'] > 0 ? quiz_icon('should') : ($chosen ? quiz_icon('selected') : ''),
				'attempt' => $icon,
				'score' => $choice,
				//'solution' =>  : ($simple_scoring ? quiz_icon('should-not') : ''),
			);
		}

		return $data;
	}

	/**
	* Order the alternatives according to the choice order stored in the database
	*
	* @param array $alternatives
	*  The alternatives to be ordered
	*/
	protected function orderAlternatives(array &$alternatives) {
		if (!$this->question->choice_random) {
			return;
		}

		$result = db_query('SELECT choice_order FROM {quiz_multichoice_user_answers} WHERE result_id = :result_id AND question_nid = :question_nid AND question_vid = :question_vid', array(':result_id' => $this->result_id, ':question_nid' => $this->question->nid, ':question_vid' => $this->question->vid))->fetchField();

		if (!$result) {
			return;
		}

		$order = explode(',', $result);
		$newAlternatives = array();

		foreach ($order as $value) {
			foreach ($alternatives as $alternative) {
				if ($alternative['id'] == $value) {
					$newAlternatives[] = $alternative;
					break;
				}
			}
		}

		$alternatives = $newAlternatives;
	}
}