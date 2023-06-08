<?php
/**
	* @file
	* Themes the question report
	*
	*/
/*
	* Available variables:
	* $form - FAPI array
	*
	* All questions are in form[x] where x is an integer.
	* Useful values:
	* $form[x]['question'] - the question as a FAPI array(usually a form field of type "markup")
	* $form[x]['score'] - the users score on the current question.(FAPI array usually of type "markup" or "textfield")
	* $form[x]['max_score'] - the max score for the current question.(FAPI array of type "value")
	* $form[x]['response'] - the users response, usually a FAPI array of type markup.
	*/
	
	//dprint_r($form);
?>
<h2><?php print t('Quiz Results'); ?></h2>
<div class="quiz-report">
	<?php foreach ($form as $key => $sub_form):
		// Don't show result for demographics question...
		if (!is_numeric($key) || isset($sub_form['#no_report']) || $sub_form['question']['#bundle'] == 'demographics') {
			continue;
		}

		unset($form[$key]);
	?>
		<div class="quiz-report-row clearfix">
			<div class="quiz-report-question dt">
				<div class="quiz-report-question-header clearfix">
					<h3><?php print t('Question ' . $key . ':') ?></h3>
					<?php print drupal_render($sub_form['score_display']); ?>
				</div>
				<?php print drupal_render($sub_form['question']); ?>
			</div>
			<div class="quiz-report-response dd">
				<!-- <h3 class="quiz-report-response-header"><?php print t('Response:') ?></h3> -->
				<?php print drupal_render($sub_form['response']); ?>
			</div>
			<div class="quiz-report-question-annotation dd">
				<h3><?php print t('Question ' . $key . ' annotation:') ?></h3>
				<?php print drupal_render($sub_form['question_annotation']); ?>
			</div>
		</div>
		<hr />
	<?php endforeach; ?>
</div>
<!-- <div class="quiz-score-submit"><?php print drupal_render_children($form); ?></div> -->
<script>
	(function ($) {
		$(function(){
			//alert('Module version.');
		})()
	})(jQuery);
</script>