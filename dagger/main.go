package main

import (
    "context"
    "fmt"
    "os"

    "dagger.io/dagger"
)

func main() {
    if err := build(context.Background()); err != nil {
        fmt.Println(err)
    }
}

func build(ctx context.Context) error {
    fmt.Println("Building with Dagger")

    client, err := dagger.Connect(ctx, dagger.WithLogOutput(os.Stdout))
    if err != nil {
        return err
    }
    defer client.Close()

    cwd, err := os.Getwd()
    if err != nil {
        fmt.Println(err)
        return err
    }
    src := client.Host().Directory(cwd + "/..")
    container := client.Host().Directory(".").DockerBuild()
    container = container.WithMountedDirectory("/workspace", src)


    build_dir := "/build"
    container = container.WithWorkdir(build_dir)
    container = container.WithExec([]string{"cmake", "/workspace"})
    container = container.WithExec([]string{"make", "-j", "8"})

    output := container.Directory(build_dir)
    _, err = output.Export(ctx, "output/")
    if err != nil {
        return err
    }
    defer client.Close()

    return nil
}
