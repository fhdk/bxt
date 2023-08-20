module.exports = {
  daisyui: {
    themes: [
      {
        mytheme: {
        


"primary": "#35bfa4",
        


"secondary": "#3daee9",
        


"accent": "#60ffca",
        


"neutral": "#fcfcfc",
        


"base-100": "#eff0f1",
        


"info": "#799CE6",
        


"success": "#27ae60",
        


"warning": "#f67400",
        


"error": "#da4453",
        },
      },
    ],
  },
  content: [
    "./src/**/*.{js,jsx,ts,tsx}",
  'node_modules/daisyui/dist/**/*.js', 'node_modules/react-daisyui/dist/**/*.js'],
    plugins: [require("@tailwindcss/typography"), require('daisyui')],
  }