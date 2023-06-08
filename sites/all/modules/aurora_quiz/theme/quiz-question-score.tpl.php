<?php if ($max_score > 0) { ?>
	<div class="quiz-report-score-container <?php print $class ?>">
		<span>
			<?php print t('Score: ') . $score . t(' of ') . $max_score; ?>
		</span>
	</div>
<? } 