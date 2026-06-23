@tool
extends TextureProgressBar

@export var progress_texture_map: GradientTexture2D


func _ready() -> void:
	value_changed.connect(lala)


func lala(value: float) -> void:
	#texture_progress = texture_progress as GradientTexture2D
	texture_progress.gradient.set_color(
		0, progress_texture_map.gradient.sample(value / max_value)
	)
