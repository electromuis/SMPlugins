This a repo with all the StepMania plugins made by me.

Building is easiest with docker, put correct StepMania plugin SDK in the plugins folder. Then compile using:
```
docker run --rm -v %cd%:/data electromuis/smsdk plugin_build --plugin [Plugin Name] --sdk [SDK Name] --compiler msvc
```

After which your dll will be in a "built" folder