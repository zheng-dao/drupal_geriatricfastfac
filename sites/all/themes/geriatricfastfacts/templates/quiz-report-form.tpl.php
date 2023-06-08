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
?>
<style type="text/css">
	h1.page-header {
		display: none;
	}
	.footer_fat.hidden-xs {
		display: none !important;
	}
</style>

<div class="quiz-report">
	<div class="quiz-report-notice">
		<p>Any short answer questions were not scored. To enhance your learning, annotated answers for all questions are provided below.</p>
	</div>
	<h2><?php print t('Quiz Results'); ?></h2>
	<!-- <div class="quiz-report-row clearfix">
		<span>Key: </span>
		<div class="quiz-score-icon should"></div><span> = Correct answer</span>
	</div> -->
	
	<?php foreach ($form as $key => $sub_form):
		// Don't show result for demographics question...
		if (!is_numeric($key) || isset($sub_form['#no_report']) || $sub_form['question']['#bundle'] == 'demographics') {
			continue;
		}

		unset($form[$key]);
	?>
	<hr/>
	<div class="quiz-report-row clearfix">
		<div class="quiz-report-question dt">
			<div class="quiz-report-question-header clearfix">
				<h3><?php print t('Question ' . $key) ?></h3>
				<?php print drupal_render($sub_form['score_display']); ?>
			</div>
			<?php print drupal_render($sub_form['question']); ?>
		</div>
		<div class="quiz-report-response dd">
			<?php print drupal_render($sub_form['response']); ?>
		</div>
		<div class="quiz-report-question-feedback dd">
			<h3 class="quiz-report-annotation-header"><?php print t('Annotated answer') ?></h3>
			<?php print drupal_render($sub_form['question_annotation']); ?>
		</div>
	</div>
	
	<?php endforeach; ?>
</div>
<a class="btn btn-default btn-lg" href="/"><span class="glyphicon glyphicon-chevron-left" aria-hidden="true"></span> Go back to the homepage</a>