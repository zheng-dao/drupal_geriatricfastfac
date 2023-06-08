(function ($) {
	console.log("trying to show panels");
	$(document).ready(function(){	
	$.ajaxSetup ({
    	// Disable caching of AJAX responses
    	cache: false
	});
	 //$('.badge').load('/fav.php?<?php print rand(); ?>');
	//$('.saved-homes-head').load('/sh.php');
	$('.badge').load('/fav.php');

	$('.node-fast-fact .panel-collapse').collapse("show");
	
	$('.accShow').on('click', function() {
	   if($(this).text() == 'Expand All') {
		   $('.node-fast-fact .panel-collapse').collapse("show");
		   $('.accShow').text('Collapse All');
	   } else {
		   
		   $('.node-fast-fact .panel-collapse').collapse("hide");
		   $('.accShow').text('Expand All');
	   }
	   return false;
	});

	// resposive tables
	  var switched = false;
  var updateTables = function() {
    if (($(window).width() < 767) && !switched ){
      switched = true;
      $("table.responsive").each(function(i, element) {
        splitTable($(element));
      });
      return true;
    }
    else if (switched && ($(window).width() > 767)) {
      switched = false;
      $("table.responsive").each(function(i, element) {
        unsplitTable($(element));
      });
    }
  };
   
  $(window).load(updateTables);
  $(window).bind("resize", updateTables);
   
	
	function splitTable(original)
	{
		original.wrap("<div class='table-wrapper' />");
		
		var copy = original.clone();
		copy.find("td:not(:first-child), th:not(:first-child)").css("display", "none");
		copy.removeClass("responsive");
		
		original.closest(".table-wrapper").append(copy);
		copy.wrap("<div class='pinned' />");
		original.wrap("<div class='scrollable' />");
	}
	
	function unsplitTable(original) {
    original.closest(".table-wrapper").find(".pinned").remove();
    original.unwrap();
    original.unwrap();
	}
	
	function touchScroll(selector) {
		
		if (isTouchDevice()) {
			var scrollStartPosY=0;
			var scrollStartPosX=0;
			$("body").delegate(selector, 'touchstart', function(e) {
				scrollStartPosY=this.scrollTop+e.originalEvent.touches[0].pageY;
				scrollStartPosX=this.scrollLeft+e.originalEvent.touches[0].pageX;
			});
			$("body").delegate(selector, 'touchmove', function(e) {
				if ((this.scrollTop < this.scrollHeight-this.offsetHeight &&
					this.scrollTop+e.originalEvent.touches[0].pageY < scrollStartPosY-5) ||
					(this.scrollTop != 0 && this.scrollTop+e.originalEvent.touches[0].pageY > scrollStartPosY+5))
						e.preventDefault();
				if ((this.scrollLeft < this.scrollWidth-this.offsetWidth &&
					this.scrollLeft+e.originalEvent.touches[0].pageX < scrollStartPosX-5) ||
					(this.scrollLeft != 0 && this.scrollLeft+e.originalEvent.touches[0].pageX > scrollStartPosX+5))
						e.preventDefault();
				this.scrollTop=scrollStartPosY-e.originalEvent.touches[0].pageY;
				this.scrollLeft=scrollStartPosX-e.originalEvent.touches[0].pageX;
			});
		}
	}
	function isTouchDevice(){
		try{
			document.createEvent("TouchEvent");
			return true;
		}catch(e){
			return false;
		}
	}
	
	touchScroll("table.responsive");
	touchScroll("header .navbar-collapse");

});
}(jQuery));