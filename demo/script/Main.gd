extends Node

@export var mob_scene: PackedScene
var score

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	#new_game()
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta: float) -> void:
	pass

func new_game() -> void:
	score = 0
	$Music.play()
	$Hud.update_score(score)
	$Hud.show_message("Get Ready")
	$Player.start($StartPosition.position)
	$StartTimer.start()

func game_over() -> void:
	$Music.stop()
	$DeathMusic.play()
	$Hud.show_game_over()
	$ScoreTimer.stop()
	$MobTimer.stop()


func _on_start_timer_timeout() -> void:
	$ScoreTimer.start()
	$MobTimer.start()

# add score
func _on_score_timer_timeout() -> void:
	score += 1
	$Hud.update_score(score)
	if not $Music.playing:
		$Music.play()

# spawn mob
func _on_mob_timer_timeout() -> void:
	var mob = mob_scene.instantiate()
	# specify mob spawn location
	var mob_spawn_location = $MobPath/MobSpawnLocation
	mob_spawn_location.progress_ratio = randf()
	mob.position = mob_spawn_location.position
	# specify mob move direction
	var direction = mob_spawn_location.rotation + PI/2
	direction += randf_range( - PI / 4, + PI / 4)
	mob.rotation = direction
	# specify mob veosity
	var velocity = Vector2(randf_range(150.0, 250.0), 0.0)
	mob.linear_velocity = velocity.rotated(direction)
	
	add_child(mob)
	pass # Replace with function body.
