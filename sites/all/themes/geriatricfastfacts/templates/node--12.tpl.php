<article id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>


  <header>
    <?php print render($title_prefix); ?>
    <?php if (!$page && $title): ?>
      <h2<?php print $title_attributes; ?>><a href="<?php print $node_url; ?>"><?php print $title; ?></a></h2>
    <?php endif; ?>
    <?php print render($title_suffix); ?>

    <?php if ($display_submitted): ?>
      <span class="submitted">
        <?php print $user_picture; ?>
        <?php print $submitted; ?>
      </span>
    <?php endif; ?>
  </header>

  <?php
    // Hide comments, tags, and links now so that we can render them later.
    hide($content['comments']);
    hide($content['links']);
    hide($content['field_tags']);
    print render($content);

	if (isset($_COOKIE['favorite_facts'])) {
		$favfacts = preg_split('/;/', $_COOKIE['favorite_facts'], -1, PREG_SPLIT_NO_EMPTY);
		
		$favfactsnids = array();
		
		foreach ($favfacts as $fact):
			$this_fact = preg_split('/\|/', $fact, -1, PREG_SPLIT_NO_EMPTY);
			$favfactsnids[] = $this_fact[1];
		endforeach;
		
		$blocklist = implode("+", $favfactsnids);
		print views_embed_view('fast_facts', 'block_1', $blocklist); 
	} else {
		print '<p>No Favorites found.  Add your favorite Fast Facts by navigating to a Fast Fact page and clicking "Add to Favorites".</p>';	
	}
	
  ?>

  <?php if (!empty($content['field_tags']) || !empty($content['links'])): ?>
    <footer>
      <?php print render($content['field_tags']); ?>
      <?php print render($content['links']); ?>
    </footer>
  <?php endif; ?>

  <?php print render($content['comments']); ?>

</article> <!-- /.node -->
