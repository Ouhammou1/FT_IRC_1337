#!/bin/bash

# 1. Configure Git user
git config --global user.name "Ouhammou2"
git config --global user.email "brahimouhammou2022@gmail.com"
echo "Git username and email set."

# 2. Switch branch to main
git branch -M main
echo "Switched branch to main."

# 3. Set SSH remote URL
git remote set-url origin git@github.com:Ouhammou1/FT_IRC_1337.git
echo "Remote set to SSH URL."

# 4. Add all files
git add .

# 5. Commit changes
git commit -m "Auto commit"
echo "Changes committed."

# 6. Push to GitHub
git push -u origin main
echo "Changes pushed to GitHub."

