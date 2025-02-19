extends ConfirmationDialog

var result: bool

signal confirmation_result(result)

func wait_for_confirmation() -> bool:
	return yield(self, "confirmation_result")

func _on_OverwriteConfirm_popup_hide():
	print("closing")
	if self.get_ok().is_pressed():
		self.result = true
	emit_signal("confirmation_result", self.result)

func _on_OverwriteConfirm_about_to_show():
	self.result = false
