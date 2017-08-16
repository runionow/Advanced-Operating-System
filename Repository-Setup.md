# Repository Setup

In this section, you will be setting up a repository that will allow
you to work with xinu. In this course we will be relying heavily on
git. If you are not familiar with this tool, you should read the first
3 chapters of the following book.  
[Pro Git](https://git-scm.com/book/en/v2)  
The VM is here - https://iu.box.com/s/8j62qjccr94u84og1ei4g5pnd94q1eb9  

## Setting up your private keys

To work efficiently with IU github, you will need to set up your
private keys. Github.com has a very good tutorial on how to set this
up. You should note that where the tutorial refers to **github.com**,
you should use **github.iu.edu**.

[generating ssh keys](https://help.github.com/articles/generating-ssh-keys/)

## Repository Creation
You should create a **PRIVATE** repository called
**OS-P536-S17** on IU Github. You will be adding the following collaborators to this repository:

* bhimebau
* wang617
* kgundlup
* jainps
* nieji

## Course Repository Duplication

The [course repository](https://github.iu.edu/SOIC-Operating-Systems/P536-S17)
should be duplicated using the steps below: 

* Change directories to a scratch area. Create a bare clone of P536-S17 repository using:

```bash
git clone --bare git@github.iu.edu:SOIC-Operating-Systems/P536-S17.git
```

* Enter the P536-S17.git directory

```bash
cd P536-S17.git
```

* Push this repo data to your repository replacing **yourreponame** with the appropriate git@github.iu.edu address. 

```bash
git push --mirror yourreponame
```

* If all went well and your shared repository has the data, you can remove the P536-S17.git directory because it is no longer needed.

```bash
cd ..
rm -rf P536-S17.git
```

## Remote Setup

You will need to keep the original course repository as an upstream
remote for your repository. This repository will be updated
periodically with new lab material. Adding an upstream remote will
allow you to keep your copy synchronized with updated lab material.

* Clone your **OS-P536-S17**.

* cd into the OS-P536-S17 directory. Add an upstream remote to your repository. (Both partners will need to do this)

```bash
git remote add upstream git@github.iu.edu:SOIC-Operating-Systems/P536-S17.git
```

* In the future, you can pull in updates to the course repository using a fetch command.

```bash
git fetch upstream
```
