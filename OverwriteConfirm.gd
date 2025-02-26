extends ConfirmationDialog

signal confirmation_result(result)

func _on_OverwriteConfirm_popup_hide():
	var result = false
	if self.get_ok().is_pressed():
		result = true
	emit_signal("confirmation_result", result)
