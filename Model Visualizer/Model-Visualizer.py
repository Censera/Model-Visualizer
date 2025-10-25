import pyray as rl

def main():
    window = rl.Vector2(900, 600)
    rl.set_config_flags(rl.FLAG_MSAA_4X_HINT)
    rl.init_window(int(window.x), int(window.y), b"Model Visualizer")
    rl.set_target_fps(30)

    model = rl.load_model(b"Models/SpikeBall.obj")
    texture = rl.load_texture(b"Textures/RandomTexture.jpg")
    model.materials[0].maps[0].texture = texture

    background_image = rl.load_image(b"Images/BackgroundImage.jpg")
    scale_factor = window.x / background_image.width
    rl.image_resize(background_image, int(window.x), int(background_image.height * scale_factor))
    background_texture = rl.load_texture_from_image(background_image)

    main_font = rl.load_font_ex(b"Fonts/MainFont.ttf", 100, None, 250)
    rl.gen_texture_mipmaps(main_font.texture)
    rl.set_texture_filter(main_font.texture, rl.TEXTURE_FILTER_BILINEAR)
    rl.gui_set_font(main_font)

    model_pos = rl.Vector3(0, 0, 0)
    model_axes = rl.Vector3(0, 0, 0)
    model_scale = rl.Vector3(0.5, 0.5, 0.5)

    camera = rl.Camera3D()
    camera.position = rl.Vector3(0, 0, 5)
    camera.target = rl.Vector3(0, 0, 0)
    camera.up = rl.Vector3(0, 1, 0)
    camera.fovy = 60
    camera.projection = rl.CAMERA_PERSPECTIVE

    rl.gui_set_style(rl.DEFAULT, rl.TEXT_COLOR_NORMAL, rl.color_to_int(rl.Color(255, 255, 255, 240)))
    rl.gui_set_style(rl.DEFAULT, rl.TEXT_SIZE, 12)
    rl.gui_set_style(rl.DEFAULT, rl.TEXT_SPACING, 0)
    rl.gui_set_style(rl.DEFAULT, rl.BASE_COLOR_NORMAL, rl.color_to_int(rl.Color(255, 255, 255, 10)))
    rl.gui_set_style(rl.DEFAULT, rl.BORDER_COLOR_NORMAL, rl.color_to_int(rl.Color(255, 255, 255, 250)))
    rl.gui_set_style(rl.DEFAULT, rl.BASE_COLOR_FOCUSED, rl.color_to_int(rl.Color(255, 255, 255, 40)))
    rl.gui_set_style(rl.DEFAULT, rl.BORDER_COLOR_FOCUSED, rl.color_to_int(rl.Color(255, 255, 255, 40)))
    rl.gui_set_style(rl.DEFAULT, rl.BORDER_WIDTH, 2)
    rl.gui_set_style(rl.DEFAULT, rl.BASE_COLOR_PRESSED, rl.color_to_int(rl.Color(255, 255, 255, 200)))
    rl.gui_set_font(main_font)

    rotation_angle = 0.0
    toggle_wires = True
    camera_mode = 0
    textfile = rl.load_file_text(b"float.c")

    while not rl.window_should_close():
        rl.begin_drawing()
        rl.clear_background(rl.BLACK)
        rl.draw_texture_v(background_texture, rl.Vector2(0, 0), rl.WHITE)

        info_fps = f"{rl.get_fps()} fps"
        info_screen = f"{rl.get_screen_width()}x{rl.get_screen_height()} Screen"
        info_render = f"{rl.get_render_width()}x{rl.get_render_height()} Render"
        info_obj = textfile.decode() if textfile else ""

        rl.draw_text_ex(main_font, b"Texture loaded successfully", rl.Vector2(10, 130), 12, 0, rl.WHITE)
        rl.draw_text_ex(main_font,
                        b"Invalid Model" if model.mesh_count == 0 else b"Model loaded successfully",
                        rl.Vector2(10, 150), 12, 0, rl.RED if model.mesh_count == 0 else rl.WHITE)
        rl.draw_text_ex(main_font,
                        b"Invalid Image" if background_image.width == 0 or background_image.height == 0 else b"Image loaded successfully",
                        rl.Vector2(10, 170), 12, 0, rl.RED if background_image.width == 0 or background_image.height == 0 else rl.WHITE)
        rl.draw_text_ex(main_font,
                        b"Invalid Font" if main_font.texture.id == 0 else b"Font loaded successfully",
                        rl.Vector2(10, 190), 12, 0, rl.RED if main_font.texture.id == 0 else rl.WHITE)

        rl.draw_text_ex(main_font, info_fps.encode(), rl.Vector2(10, 10), 50, 0, rl.WHITE)
        rl.draw_text_ex(main_font, info_screen.encode(), rl.Vector2(200, 24), 14, 0, rl.WHITE)
        rl.draw_text_ex(main_font, info_render.encode(), rl.Vector2(200, 44), 14, 0, rl.WHITE)
        rl.draw_text_ex(main_font, info_obj.encode(), rl.Vector2(10, 220), 10, 0, rl.Color(255, 255, 255, 200))

        modes = [
            b"Camera Mode: CAMERA_ORBITAL",
            b"Camera Mode: CAMERA_FREE",
            b"Camera Mode: CAMERA_CUSTOM",
            b"Camera Mode: CAMERA_FIRST_PERSON",
            b"Camera Mode: CAMERA_THIRD_PERSON"
        ]
        rl.draw_text_ex(main_font, modes[camera_mode], rl.Vector2(600, 20), 12, 0, rl.WHITE)

        rl.begin_mode3d(camera)
        if toggle_wires:
            rl.draw_model_ex(model, model_pos, model_axes, rotation_angle, model_scale, rl.WHITE)
        else:
            rl.draw_model_wires_ex(model, model_pos, model_axes, rotation_angle, model_scale, rl.WHITE)
        rl.end_mode3d()

        # Camera controls
        if rl.is_key_down(rl.KEY_W) or rl.is_key_down(rl.KEY_Z):
            rl.gui_button(rl.Rectangle(10, 80, 50, 30), b"W/Z")
        if rl.is_key_down(rl.KEY_S):
            rl.gui_button(rl.Rectangle(10, 80, 50, 30), b"S")
        if rl.is_key_down(rl.KEY_D):
            rl.gui_button(rl.Rectangle(10, 80, 50, 30), b"D")
        if rl.is_key_down(rl.KEY_A) or rl.is_key_down(rl.KEY_Q):
            rl.gui_button(rl.Rectangle(10, 80, 50, 30), b"Q/A")

        if rl.gui_button(rl.Rectangle(600, 500, 120, 30), b"Reset"):
            model_pos = rl.Vector3(0, 0, 0)
            model_axes = rl.Vector3(0, 0, 0)
            model_scale = rl.Vector3(0.5, 0.5, 0.5)
            camera.position = rl.Vector3(0, 0, 5)
            camera.target = rl.Vector3(0, 0, 0)
            camera.up = rl.Vector3(0, 1, 0)
            rotation_angle = 0
            camera.fovy = 60

        if rl.gui_button(rl.Rectangle(730, 500, 120, 30), b"Show Wires"):
            toggle_wires = not toggle_wires

        if rl.gui_button(rl.Rectangle(600, 550, 250, 30), b"Camera"):
            camera_mode = (camera_mode + 1) % 5

        rl.end_drawing()

        if camera_mode == 0:
            rl.update_camera(camera, rl.CAMERA_ORBITAL)
        elif camera_mode == 1:
            rl.update_camera(camera, rl.CAMERA_FREE)
        elif camera_mode == 2:
            rl.update_camera(camera, rl.CAMERA_CUSTOM)
        elif camera_mode == 3:
            rl.update_camera(camera, rl.CAMERA_FIRST_PERSON)
        elif camera_mode == 4:
            rl.update_camera(camera, rl.CAMERA_THIRD_PERSON)

    rl.unload_texture(background_texture)
    rl.unload_model(model)
    rl.unload_image(background_image)
    rl.unload_font(main_font)
    rl.close_window()

if __name__ == "__main__":
    main()
