# SPW
Speech Processing Window(Streaming Speech enhancement and Speech-to-Text system for Otolaryngology reception counter)

## Dependecy

### Raspberry-pi

see https://github.com/kooBH/SPW/issues/1  and https://github.com/kooBH/SPW/issues/2  

### vcpkg
+ linux  
```
lib/vcpkg/bootstrap-vcpkg.sh
```

+ windows
```
lib/vcpkg/bootstrap-vcpkg.bat
```

## Build

+ windows
```
cmake -S. -B bin "-DCMAKE_TOOLCHAIN_FILE=<absolute-path-to-vcpkg>/vcpkg/scripts/buildsystems/vcpkg.cmake"
cmake --build .build
```

## Credential

```
export GOOGLE_APPLICATION_CREDENTIALS=/path/to/your/credentials-key.json
```

## REFERENCE
[how to call Google Cloud services from C++](https://github.com/GoogleCloudPlatform/cpp-samples)        
[vcpkg](https://github.com/microsoft/vcpkg)    
[Supported Languages](https://cloud.google.com/speech-to-text/docs/speech-to-text-supported-languages?hl=ko)     
[Cloud Text-to-Speech API C++ Client](https://googleapis.dev/cpp/google-cloud-texttospeech/HEAD/)     
