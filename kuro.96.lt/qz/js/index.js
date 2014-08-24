$(document).ready(function() {
	var ticket = $('input[name=ticket]').val();
	var beginBtn = $('#beginBtn');
	var judgmentBtn = $('#judgmentBtn');

	beginBtn.button();
	beginBtn.click(function() {
		begin(beginBtn);
	});
	beginBtn.button('enable');
	if (ticket == '') {
		var dialog = $('#loginDialog');
		dialog.dialog({
			resizable: false,
			modal: true,
			buttons: {
				'Login': function() {
					var settings = makeSettings(this, 'auth', ['user', 'pass']);
					ajax(settings, function(map) {
						if (map.ticket !== undefined) {
							$('input[name=ticket]').val(map.ticket);
							dialog.dialog('close');
							init();
						}
					});
				}
			}
		 });
	} else {
		init();
	}
	$('#answers ol').selectable({
		stop: function() {
			var li = $('#answers li.ui-selected');

			if (0 == li.length) {
				judgmentBtn.button('disable');
			} else {
				judgmentBtn.button('enable');
			}
		}
	});
	judgmentBtn.button();
	judgmentBtn.click(function() {
		judge(this);
	});
	$('#nextBtn').click(function() {
		var index = parseInt($('input[name=index]').val()) + 1;
		var max = $('[name=numOfQuestion]').val();

//$('#judgment').text('index:' + index + '/' + max);
		if (max < index) {
			$(this).attr('disabled', 'disabled');
			$('#judgment').html('<span>End.</span>');
			$('select[name=catID]').selectmenu('enable');
			$('select[name=numOfQuestion]').selectmenu('enable');
			beginBtn.button('enable');
			return;
		}
		$('input[name=index]').val(index);
		showQuestion();
	});
	// [▼]ボタン
	$('#resultIcon').click(function() {
		var btn = $(this);
		if (btn.hasClass('ui-icon-triangle-1-e')) {
			btn.removeClass('ui-icon-triangle-1-e');
			btn.addClass('ui-icon-triangle-1-s');
		} else {
			btn.removeClass('ui-icon-triangle-1-s');
			btn.addClass('ui-icon-triangle-1-e');
		}
		$('#resultText').toggle();
	});
});
function showProgress() {
	var index = parseInt($('input[name=index]').val());
	var rights = $('#progress td.rightAnswer').length;
	var ratio = rights * 100 / index;

	$('input[name=ratio]').val(ratio + '%')
}
/**
 * 判定結果を表示.
 */
function judge(obj) {
	var li = $('#answers li.ui-selected');

	if (li.length != 1) {
		judgment('Please select only one answer.');
		return;
	}
	var seq = li.attr('seq');
	$('input[name=seq]').val(seq);
	var settings = makeSettings(obj, 'judge', ['ticket', 'qID', 'seq']);
	ajax(settings, function(map) {
		var index = parseInt($('input[name=index]').val()) - 1;
		var td = $('#progress td').eq(index);

		judgment(map.result);
		td.removeClass('rightAnswer');
		td.removeClass('incorrect');
		if (map.judge) {
			td.addClass('rightAnswer');
		} else {
			td.addClass('incorrect');
		}
		td.addClass('done');
		showProgress();
	});
}
function judgment(msg) {
	var judgment = $('#judgment');

	judgment.hide();
	judgment.text(msg);
	judgment.show('bounce', { times: 3 }, 'slow');
}
function convertQuestion(question) {
	var result = question.replace('誤っている', '<span class="mistake">誤っている</span>');
	return result;
}
/**
 * 質問と選択肢を生成.
 */
function showQuestion() {
	var beginBtn = $('#beginBtn');
	var list = beginBtn.prop('list');
	var index = $('input[name=index]').val() - 1;
	var qID = list[index];

	$('input[name=qID]').val(list[index]);
	var settings = makeSettings(beginBtn, 'next', ['ticket', 'catID', 'qID']);
	ajax(settings, function(map) {
		var question = convertQuestion(map['question']);
		var questionText = $('#questionText');
		var ol = $('#answers ol');

		$('input[name=qID]').val(map['qID']);
		if (questionText.hasClass('ui-resizable')) {
			questionText.resizable('destroy');
		}
		questionText.html(question);
		questionText.resizable();
		questionText.show();
		ol.empty();
		$(map.answer).each(function(iy, rec) {
			var li = $('<li>' + rec.content + '</li>');
			li.attr('seq', rec.seq);
			ol.append(li);
		});
		ol.selectable('refresh');
		$('#judgmentBtn').attr('disabled', 'disabled');
		$('#judgmentBtn').show();
		$('#judgment').empty();
	});
}
/**
 * 開始.
 * @param beginBtn
 */
function begin(beginBtn) {
	var max = parseInt($('[name=numOfQuestion]').val());
	var progress = $('#progress');

	progress.empty();
	for (var cnt = 1; cnt <= max; cnt++) {
		if ((cnt - 1) % 50 == 0) {
			tr = $('<tr></tr>')
			progress.append(tr);
		}
		var td = $('<td class="numeric">' + cnt + '</td>')
		tr.append(td);
	}
	$('select[name=catID]').selectmenu('disable');
	$('select[name=numOfQuestion]').selectmenu('disable');
	$('#beginBtn').button('disable');
	$('input[name=index]').val(1);
	$('input[name=rights]').val(0);
	showProgress();
	//
	var settings = makeSettings(beginBtn, 'begin', ['ticket', 'catID', 'numOfQuestion']);
	ajax(settings, function(map) {
		beginBtn.prop('list', map.list);
		$('#nextBtn').removeAttr('disabled');
		$('#game').show();
		showQuestion();
	});
}
function init() {
	var select = $('select[name=catID]');
	var settings = makeSettings(select, 'list', ['ticket']);

	ajax(settings, function(map) {
		$(map.list).each(function(iy, rec) {
			var text = rec.catName + ' (' + rec.cnt + ')';
			var opt = $('<option>' + text + '</option>');
			opt.attr('value', rec.catID);
			opt.attr('cnt', rec.cnt);
			select.append(opt);
		});
		select.selectmenu({
			change: function() {
				categoryChanged(select);
			}
		});
		select.selectmenu('enable');
		categoryChanged(select);
	});
}
function categoryChanged(select) {
	var opt = select.find('option:selected');
	var max = opt.attr('cnt');
	var select = $('select[name=numOfQuestion]');
	var beginBtn = $('#beginBtn');

	select.empty();
	$([10, 20, 50, 100]).each(function(ix, val) {
		if (max < val) {
			return false;
		}
		var opt = $('<option>' + val + '</option>');
		opt.attr('value', val);
		select.append(opt);
	});
	select.selectmenu();
	select.selectmenu('enable');
}
function makeSettings(base, act, nameList) {
	var tr = $(base).parents('form:first');
	var data = {act: act};
	$(nameList).each(function(ix, name) {
		var isCheckbox = 0 < tr.find('[name=' + name + ']:checkbox').length;
		var isSelect = 0 < tr.find('[name=' + name + ']').find('option:selected').length;
		var elm = tr.find('[name=' + name + ']');
		var val = elm.val();
		if (isCheckbox) {
			data[name] = elm.get(0).checked;
			return;
		}
		if (isSelect) {
			data[name] = elm.find('option:selected').attr('value');
			return;
		}
		if (typeof val !== 'undefined') {
			data[name] = val;
			return;
		}
		var selector = '#mainForm input[name=' + name + ']';
		data[name] = $(selector).val();
	});
	return {type:'POST', url:'./act/', data: data};
}
function ajax(settings, func) {
	$.ajax(settings).done(function(json) {
		$('#resultText').val(json);
		var map = $.parseJSON(json);
		if (map.cause !== undefined) {
			alert(map.cause);
			return;
		}
		func(map);
	});
}
