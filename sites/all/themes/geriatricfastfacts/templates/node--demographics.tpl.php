<script>
	(function ($) {
		$(document).ready(function(){
			$.ajaxSetup ({
				// Disable caching of AJAX responses
				cache: false
			});
			$('.extlink a').attr ('target', '_blank');
			if($.cookie('favorite_facts')) {
				//cookie exists.
				var checkme = $.cookie('favorite_facts');
				if (checkme.indexOf("<?php print $saveme; ?>")!=-1) {
					$( "#faved").addClass( "glyphicon-star" );
					$('.addrem').text('Remove from Favorites');
				} else {
					$( "#faved").addClass( "glyphicon-star-empty" );
				}
			} else {
				$( "#faved").addClass( "glyphicon-star-empty" );
			}
			$(".addrem").click(function(){
				//add this to favorites/check existing favs
				if($("#faved").hasClass('glyphicon-star')) {	//is already faved
					if ($.cookie('favorite_facts')) {
						var checkme = $.cookie('favorite_facts');
						if (checkme.indexOf("<?php print $saveme; ?>")!=-1) {
							var tempcook = $.cookie('favorite_facts');
							var cookval = tempcook.replace(/<?php print str_replace("|", "\|", str_replace("/", "\/", $saveme)); ?>/i, "");
							if (cookval == "") {
								cookval = null;
							}
							ga('send', 'event', 'Favorites', 'Removed', '<?php print $title; ?> - #<?php print $node->field_fast_fact_number['und'][0]['value']; ?>');
							$(this).text('Add to Favorites');
							$("#faved").removeClass( "glyphicon-star" );
							$("#faved").addClass( "glyphicon-star-empty" );
						}
					}
				} else {
					if($.cookie('favorite_facts')) {
						//cookie exists.
						var checkme = $.cookie('favorite_facts');
						if (checkme.indexOf("<?php print $saveme; ?>")!=-1) {
							var tempcook = $.cookie('favorite_facts');
							var cookval = tempcook.replace(/<?php print str_replace("|", "\|", str_replace("/", "\/", $saveme)); ?>/i, "");
							if (cookval == "") {
								cookval = null;
							}
							ga('send', 'event', 'Favorites', 'Removed', '<?php print $title; ?> - #<?php print $node->field_fast_fact_number['und'][0]['value']; ?>');
							$(this).text('Add to Favorites');
							$( "#faved").removeClass( "glyphicon-star" );
							$( "#faved").addClass( "glyphicon-star-empty" );
						} else {
							var tempcook = $.cookie('favorite_facts');
							var cookval = tempcook.concat( '<?php print $saveme; ?>');
							ga('send', 'event', 'Favorites', 'Added', '<?php print $title; ?> - #<?php print $node->field_fast_fact_number['und'][0]['value']; ?>');
							$(this).text('Remove from Favorites');	
							$( "#faved").addClass( "glyphicon-star" );
							$( "#faved").removeClass( "glyphicon-star-empty" );
						}
					} else {
						var tempcook = "";
						var cookval = tempcook.concat( '<?php print $saveme; ?>');
						ga('send', 'event', 'Favorites', 'Added', '<?php print $title; ?> - #<?php print $node->field_fast_fact_number['und'][0]['value']; ?>');
						$(this).text('Remove from Favorites');
						$( "#faved").addClass( "glyphicon-star" );
						$( "#faved").removeClass( "glyphicon-star-empty" );
					}
				}
				$.cookie('favorite_facts', cookval, { path: '/', expires: 1800 });
				$('.badge').load('/fav.php?<?php print rand(); ?>');
			});
		});
	}(jQuery));
</script>
<article id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>
	<div class="favorite-toggle clearfix">
		<a href="#" id="favthis"><span id="faved" class="glyphicon"></span> <span class="addrem">Add to Favorites</span></a><br/>
		<a class="accShow">Expand All</a>
	</div>
	<div class="acc-expand clearfix"></div>
	<header>
		<?php print render($title_prefix); ?>
		<?php if (!$page && $title || true): ?>
			<div class="gff_title">
				<h1<?php print $title_attributes; ?>><?php print $title; ?> - #<?php print $node->field_fast_fact_number['und'][0]['value']; ?></h1>
				<?php if (isset($content['field_link_to_quiz'])) { ?>
					<a id="btnTakeQuiz" class="btn btn-default btn-lg" href="<?php print $content['field_link_to_quiz'][0]['#markup']; ?>"><span class="glyphicon glyphicon-check" aria-hidden="true"></span> Take Quiz</a>
				<?php } ?>
			</div>
		<?php endif; ?>
		<?php print render($title_suffix); ?>
		<?php if ($display_submitted): ?>
			<span class="submitted">
				<?php print $user_picture; ?>
				<?php print $submitted; ?>
			</span>
		<?php endif; ?>
	</header>
	<section>
		<?php
			// Hide comments, tags, and links now so that we can render them later.
			hide($content['comments']);
			hide($content['links']);
			hide($content['field_tags']);
			#print render($content);
			hide($content);
			#print render($title);
			#print render($content['field_fast_fact_number']);
			print render($content['body']);
		?>
		<div class="" id="fastfactaccordion">
			<?php if (isset($content['field_context'])) { ?>
				<div class="accordion">
					<div class="accordion-heading">
						<h2><a class="accordion-toggle collapsed" data-toggle="collapse" href="#field_context">Context<span class="glyphicon glyphicon-chevron-down pull-right"></span></a></h2>
					</div>
					<div id="field_context" class="panel-collapse collapse">
						<div class="accordion-body">
							<?php print render($content['field_context']); ?>
						</div>
					</div>
				</div>
			<?php } ?>
			<?php if (isset($content['field_action'])) { ?>
				<div class="accordion">
					<div class="accordion-heading">
						<h2><a class="accordion-toggle collapsed" data-toggle="collapse" href="#field_action">Action<span class="glyphicon glyphicon-chevron-down pull-right"></span></a></h2>
					</div>
					<div id="field_action" class="panel-collapse collapse">
						<div class="accordion-body">
							<?php print render($content['field_action']); ?>
						</div>
					</div>
				</div>
			<?php } ?>
			<?php if (isset($content['field_assessment'])) { ?>
				<div class="accordion">
					<div class="accordion-heading">
						<h2><a class="accordion-toggle collapsed" data-toggle="collapse" href="#field_assessment">Assessment<span class="glyphicon glyphicon-chevron-down pull-right"></span></a></h2>
					</div>
					<div id="field_assessment" class="panel-collapse collapse">
						<div class="accordion-body">
							<div class="extlink">
								<?php print render($content['field_assessment']); ?>
								<?php print render($content['field_assessment_images']); ?>
							</div>
						</div>
					</div>
				</div>
			<?php } ?>
			<?php if (isset($content['field_definition'])) { ?>
				<div class="accordion">
					<div class="accordion-heading">
						<h2><a class="accordion-toggle collapsed" data-toggle="collapse" href="#field_definition">Definition<span class="glyphicon glyphicon-chevron-down pull-right"></span></a></h2>
					</div>
					<div id="field_definition" class="panel-collapse collapse">
						<div class="accordion-body">
							<?php print render($content['field_definition']); ?>
						</div>
					</div>
				</div>
			<?php } ?>
			<?php if (isset($content['field_incidence'])) { ?>
				<div class="accordion">
					<div class="accordion-heading">
						<h2><a class="accordion-toggle collapsed" data-toggle="collapse" href="#field_incidence">Incidence / Prevalence<span class="glyphicon glyphicon-chevron-down pull-right"></span></a></h2>
					</div>
					<div id="field_incidence" class="panel-collapse collapse">
						<div class="accordion-body">
							<div class="extlink">
								<?php print render($content['field_incidence']); ?>
								<?php print render($content['field_incidence_image']); ?>
							</div>
						</div>
					</div>
				</div>
			<?php } ?>
			<?php if (isset($content['field_underlying_sciences']) || isset($content['field_science'])) { ?>
				<div class="accordion">
					<div class="accordion-heading">
						<h2><a class="accordion-toggle collapsed" data-toggle="collapse" href="#field_underlying_sciences">Underlying Sciences<span class="glyphicon glyphicon-chevron-down pull-right"></span></a></h2>
					</div>
					<div id="field_underlying_sciences" class="panel-collapse collapse">
						<div class="accordion-body">
							<div class="extlink">
								<?php print render($content['field_underlying_sciences']); ?>
								<?php print render($content['field_underlying_science_image']); ?>
								<?php if (isset($content['field_science'])) { ?>
									<h4>Science Principles</h4>
									<?php print render($content['field_science']); ?>
								<?php } ?>
							</div>
						</div>
					</div>
				</div>
			<?php } ?>
			<?php if (isset($content['field_background'])) { ?>
				<div class="accordion">
					<div class="accordion-heading">
						<h2><a class="accordion-toggle collapsed" data-toggle="collapse" href="#field_background">Background<span class="glyphicon glyphicon-chevron-down pull-right"></span></a></h2>
					</div>
					<div id="field_background" class="panel-collapse collapse">
						<div class="accordion-body">
							<?php print render($content['field_background']); ?>
						</div>
					</div>
				</div>
			<?php } ?>
			<?php if (isset($content['field_etiologies'])) { ?>
				<div class="accordion">
					<div class="accordion-heading">
						<h2><a class="accordion-toggle collapsed" data-toggle="collapse" href="#field_etiologies">Etiologies<span class="glyphicon glyphicon-chevron-down pull-right"></span></a></h2>
					</div>
					<div id="field_etiologies" class="panel-collapse collapse">
						<div class="accordion-body">
							<?php print render($content['field_etiologies']); ?>
						</div>
					</div>
				</div>
			<?php } ?>
			<?php if (isset($content['field_guideline'])) { ?>
				<div class="accordion">
					<div class="accordion-heading">
						<h2><a class="accordion-toggle collapsed" data-toggle="collapse" href="#field_guideline">Guideline<span class="glyphicon glyphicon-chevron-down pull-right"></span></a></h2>
					</div>
					<div id="field_guideline" class="panel-collapse collapse">
						<div class="accordion-body">
							<?php print render($content['field_guideline']); ?>
						</div>
					</div>
				</div>
			<?php } ?>
			<?php if (isset($content['field_objectives'])) { ?>
				<div class="accordion">
					<div class="accordion-heading">
						<h2><a class="accordion-toggle collapsed" data-toggle="collapse" href="#field_objectives">Objectives<span class="glyphicon glyphicon-chevron-down pull-right"></span></a></h2>
					</div>
					<div id="field_objectives" class="panel-collapse collapse">
						<div class="accordion-body">
							<?php print render($content['field_objectives']); ?>
						</div>
					</div>
				</div>
			<?php } ?>
			<?php if (isset($content['field_system']) || isset($content['field_topic']) || isset($content['field_acgme_c'])) { ?>
				<div class="accordion">
					<div class="accordion-heading">
						<h2><a class="accordion-toggle collapsed" data-toggle="collapse" href="#field_system">Keywords/Tags<span class="glyphicon glyphicon-chevron-down pull-right"></span></a></h2>
					</div>
					<div id="field_system" class="panel-collapse collapse">
						<div class="accordion-body">
							<div class="extlink">
								<?php if (isset($content['field_system'])) { ?>
									<h4>Review of Systems (ROS)</h4>
									<?php print render($content['field_system']); ?>
								<?php } ?>
								<?php if (isset($content['field_topic'])) { ?>
									<h4>Geriatric Topics</h4>
									<?php print render($content['field_topic']); ?>
								<?php } ?>
								<?php if (isset($content['field_acgme_c'])) { ?>
									<h4>ACGME Compentencies</h4>
									<?php print render($content['field_acgme_c']); ?>
								<?php } ?>
								<?php if (isset($content['field_science'])) { ?>
									<h4>Science Principles</h4>
									<?php print render($content['field_science']); ?>
								<?php } ?>
							</div>
						</div>
					</div>
				</div>
			<?php } ?>
			<?php if (isset($content['field_references'])) { ?>
				<div class="accordion">
					<div class="accordion-heading">
						<h2><a class="accordion-toggle collapsed" data-toggle="collapse" href="#field_references">References<span class="glyphicon glyphicon-chevron-down pull-right"></span></a></h2>
					</div>
					<div id="field_references" class="panel-collapse collapse">
						<div class="accordion-body">
							<?php print render($content['field_references']); ?>
						</div>
					</div>
				</div>
			<?php } ?>
			<?php print render($content['rate_fast_fact']); ?>
			<?php if (isset($content['field_authors'])) { ?>
				<div class="accordion">
					<div class="accordion-heading">
						<h2><a class="accordion-toggle collapsed" data-toggle="collapse" href="#field_authors">Authors<span class="glyphicon glyphicon-chevron-down pull-right"></span></a></h2>
					</div>
					<div id="field_authors" class="panel-collapse collapse">
						<div class="accordion-body">
							<?php print render($content['field_authors']); ?>
						</div>
					</div>
				</div>
			<?php } ?>
			<?php if (isset($content['field_author_affiliation'])) { ?>
				<div class="accordion">
					<div class="accordion-heading">
						<h2><a class="accordion-toggle collapsed" data-toggle="collapse" href="#field_author_affiliation">Author Affiliation<span class="glyphicon glyphicon-chevron-down pull-right"></span></a></h2>
					</div>
					<div id="field_author_affiliation" class="panel-collapse collapse">
						<div class="accordion-body">
							<?php print render($content['field_author_affiliation']); ?>
						</div>
					</div>
				</div>
			<?php } ?>
			<?php if (isset($content['field_copyright'])) { ?>
				<div class="accordion">
					<div class="accordion-heading">
						<h2><a class="accordion-toggle collapsed" data-toggle="collapse" href="#field_copyright">Copyright<span class="glyphicon glyphicon-chevron-down pull-right"></span></a></h2>
					</div>
					<div id="field_copyright" class="panel-collapse collapse">
						<div class="accordion-body">
							<?php print render($content['field_copyright']); ?>
						</div>
					</div>
				</div>
			<?php } ?>
		</div>
		<div class="clearfix pull-right"><a class="accShow">Expand All</a></div>
		<?php
			#print render($content['rate_fast_fact']);
			#print render($content['rate_fast_facts_percent']);
			print render($content['sharethis']);
		?>
		<div class="report-problem">
			<a href="/fast-fact-feedback?title=<?php print urlencode($node->title); ?>">Report a problem about this Fast Fact</a>
		</div>
	</section>
	<?php //print render($content['comments']); ?>
</article> <!-- /.node -->