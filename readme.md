![yume_os](yume_os.gif)

<!-- # yume -->

```
my dream mp3 player made with an esp32
```

library manager here: [yume_manager](https://github.com/deltea/yume_manager)

## known issues

- [ ] sd card randomly doesn't initialize
  - possibly contacts on sd card dirty
- [ ] constant droning sound in the background
- [ ] track progress time is a few seconds over the actual length
- [x] randomly crashing out when switching song because of "guru meditation"
  - problem was loading cover art on stack instead of heap
